import numpy as np
import tensorflow as tf
from tensorflow.keras import Model
from tensorflow.keras.layers import (
  Add,
  Concatenate,
  Conv2D,
  Input,
  Lambda,
  LeakyReLU,
  UpSampling2D,
  ZeroPadding2D,
  BatchNormalization,
)
from tensorflow.keras.regularizers import l2

YOLO_MAX_BOXES = 100
YOLO_IOU_THRESHOLD = 0.5
YOLO_SCORE_THRESHOLD = 0.5

yolo_anchors = np.array([(10, 13), (16, 30), (33, 23), (30, 61), (62, 45),
                         (59, 119), (116, 90), (156, 198), (373, 326)],
                        np.float32) / 416
yolo_anchor_masks = np.array([[6, 7, 8], [3, 4, 5], [0, 1, 2]])


def DarknetConv(x, filters, size, strides=1, batch_norm=True):
  if strides == 1:
    padding = 'same'
  else:
    x = ZeroPadding2D(((1, 0), (1, 0)))(x)  # top left half-padding
    padding = 'valid'
  x = Conv2D(filters=filters, kernel_size=size,
             strides=strides, padding=padding,
             use_bias=not batch_norm, kernel_regularizer=l2(0.0005))(x)
  if batch_norm:
    x = BatchNormalization()(x)
    x = LeakyReLU(alpha=0.1)(x)
  return x


def DarknetResidual(x, filters):
  prev = x
  x = DarknetConv(x, filters // 2, 1)
  x = DarknetConv(x, filters, 3)
  x = Add()([prev, x])
  return x


def DarknetBlock(x, filters, blocks):
  x = DarknetConv(x, filters, 3, strides=2)
  for _ in range(blocks):
    x = DarknetResidual(x, filters)
  return x


def Darknet(name=None):
  x = inputs = Input([None, None, 3])
  x = DarknetConv(x, 32, 3)
  x = DarknetBlock(x, 64, 1)
  x = DarknetBlock(x, 128, 2)
  x = x_36 = DarknetBlock(x, 256, 8)
  x = x_61 = DarknetBlock(x, 512, 8)
  x = DarknetBlock(x, 1024, 4)
  return tf.keras.Model(inputs, (x_36, x_61, x), name=name)


def YoloConv(filters, name=None):
  def yolo_conv(x_in):
    if isinstance(x_in, tuple):
      inputs = Input(x_in[0].shape[1:]), Input(x_in[1].shape[1:])
      x, x_skip = inputs

      x = DarknetConv(x, filters, 1)
      x = UpSampling2D(2)(x)
      x = Concatenate()([x, x_skip])
    else:
      x = inputs = Input(x_in.shape[1:])

    x = DarknetConv(x, filters, 1)
    x = DarknetConv(x, filters * 2, 3)
    x = DarknetConv(x, filters, 1)
    x = DarknetConv(x, filters * 2, 3)
    x = DarknetConv(x, filters, 1)
    return Model(inputs, x, name=name)(x_in)
  return yolo_conv


def YoloOutput(filters, anchors, classes, name=None):
  def yolo_output(x_in):
    x = inputs = Input(x_in.shape[1:])
    x = DarknetConv(x, filters * 2, 3)
    x = DarknetConv(x, anchors * (classes + 5), 1, batch_norm=False)
    x = Lambda(lambda x: tf.reshape(x, (-1, tf.shape(x)[1], tf.shape(x)[2],
                                        anchors, classes + 5)))(x)
    return tf.keras.Model(inputs, x, name=name)(x_in)
  return yolo_output


def broadcast_iou(box_1, box_2):
  box_1 = tf.expand_dims(box_1, -2)
  box_2 = tf.expand_dims(box_2, 0)

  new_shape = tf.broadcast_dynamic_shape(tf.shape(box_1), tf.shape(box_2))
  box_1 = tf.broadcast_to(box_1, new_shape)
  box_2 = tf.broadcast_to(box_2, new_shape)

  int_w = tf.maximum(tf.minimum(box_1[..., 2], box_2[..., 2]) -
                     tf.maximum(box_1[..., 0], box_2[..., 0]), 0)
  int_h = tf.maximum(tf.minimum(box_1[..., 3], box_2[..., 3]) -
                     tf.maximum(box_1[..., 1], box_2[..., 1]), 0)
  int_area = int_w * int_h
  box_1_area = (box_1[..., 2] - box_1[..., 0]) * \
    (box_1[..., 3] - box_1[..., 1])
  box_2_area = (box_2[..., 2] - box_2[..., 0]) * \
    (box_2[..., 3] - box_2[..., 1])
  return int_area / (box_1_area + box_2_area - int_area)


def yolo_boxes(pred, anchors, classes):
    grid_size = tf.shape(pred)[1:3]
    box_xy, box_wh, objectness, class_probs = tf.split(
      pred, (2, 2, 1, classes), axis=-1)

    box_xy = tf.sigmoid(box_xy)
    objectness = tf.sigmoid(objectness)
    class_probs = tf.sigmoid(class_probs)
    pred_box = tf.concat((box_xy, box_wh), axis=-1)

    grid = tf.meshgrid(tf.range(grid_size[1]), tf.range(grid_size[0]))
    grid = tf.expand_dims(tf.stack(grid, axis=-1), axis=2)

    box_xy = (box_xy + tf.cast(grid, tf.float32)) / \
      tf.cast(grid_size, tf.float32)
    box_wh = tf.exp(box_wh) * anchors

    box_x1y1 = box_xy - box_wh / 2
    box_x2y2 = box_xy + box_wh / 2
    bbox = tf.concat([box_x1y1, box_x2y2], axis=-1)

    return bbox, objectness, class_probs, pred_box


def yolo_nms(outputs, anchors, masks, classes):
  b, c, t = [], [], []

  for o in outputs:
    b.append(tf.reshape(o[0], (tf.shape(o[0])[0], -1, tf.shape(o[0])[-1])))
    c.append(tf.reshape(o[1], (tf.shape(o[1])[0], -1, tf.shape(o[1])[-1])))
    t.append(tf.reshape(o[2], (tf.shape(o[2])[0], -1, tf.shape(o[2])[-1])))

  bbox = tf.concat(b, axis=1)
  confidence = tf.concat(c, axis=1)
  class_probs = tf.concat(t, axis=1)

  scores = confidence * class_probs
  boxes, scores, classes, valid_detections = tf.image.combined_non_max_suppression(
    boxes=tf.reshape(bbox, (tf.shape(bbox)[0], -1, 1, 4)),
    scores=tf.reshape(
      scores, (tf.shape(scores)[0], -1, tf.shape(scores)[-1])),
    max_output_size_per_class=YOLO_MAX_BOXES,
    max_total_size=YOLO_MAX_BOXES,
    iou_threshold=YOLO_IOU_THRESHOLD,
    score_threshold=YOLO_SCORE_THRESHOLD
  )

  return boxes, scores, classes, valid_detections


def YOLOv3(num_classes=80, size=None, channels=3, 
           anchors=yolo_anchors, masks=yolo_anchor_masks):
  x = inputs = Input([size, size, channels], name='input')

  x_36, x_61, x = Darknet(name='yolo_darknet')(x)

  x = YoloConv(512, name='yolo_conv_0')(x)
  output_0 = YoloOutput(512, len(masks[0]), num_classes, name='yolo_output_0')(x)

  x = YoloConv(256, name='yolo_conv_1')((x, x_61))
  output_1 = YoloOutput(256, len(masks[1]), num_classes, name='yolo_output_1')(x)

  x = YoloConv(128, name='yolo_conv_2')((x, x_36))
  output_2 = YoloOutput(128, len(masks[2]), num_classes, name='yolo_output_2')(x)

  boxes_0 = Lambda(lambda x: yolo_boxes(x, anchors[masks[0]], num_classes),
                   name='yolo_boxes_0')(output_0)
  boxes_1 = Lambda(lambda x: yolo_boxes(x, anchors[masks[1]], num_classes),
                   name='yolo_boxes_1')(output_1)
  boxes_2 = Lambda(lambda x: yolo_boxes(x, anchors[masks[2]], num_classes),
                   name='yolo_boxes_2')(output_2)

  outputs = Lambda(lambda x: yolo_nms(x, anchors, masks, num_classes),
                   name='yolo_nms')((boxes_0[:3], boxes_1[:3], boxes_2[:3]))

  return Model(inputs, outputs, name='yolov3')