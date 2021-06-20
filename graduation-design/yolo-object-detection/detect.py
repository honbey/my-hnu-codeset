import cv2
import numpy as np
import tensorflow as tf
from timeit import default_timer as timer

from yolo import YOLOv3

NUM_CLASSES = 3
CLASSES = './model_data/obj.names'
WEIGHTS = './model_data/yolov3.tf'
SIZE = 416
# VIDEO = 'bilibili.mp4'
# VIDEO = 'rtsp://192.168.3.13:8001/h264_ulaw.sdp'
VIDEO = '0'
OUTPUT = None
OUTPUT_FORMAT = 'XVID'
COLORS = [(255, 144, 30), (0, 0, 255), (0, 255, 0)]


def transform_images(x_train, size):
  x_train = tf.image.resize(x_train, (size, size))
  x_train = x_train / 255
  return x_train


def draw_outputs(img, outputs, class_names, colors):
  boxes, objectness, classes, nums = outputs
  boxes, objectness, classes, nums = boxes[0], objectness[0], classes[0], nums[0]
  wh = np.flip(img.shape[0:2])
  for i in range(nums):
    x1y1 = tuple((np.array(boxes[i][0:2]) * wh).astype(np.int32))
    x2y2 = tuple((np.array(boxes[i][2:4]) * wh).astype(np.int32))
    thickness = round(0.001 * max(img.shape[0:2])) + 1
    img = cv2.rectangle(img, x1y1, x2y2, colors[int(classes[i])], thickness)
    t_size = cv2.getTextSize(class_names[int(classes[i])],
              cv2.FONT_HERSHEY_SIMPLEX,
              fontScale=thickness/3, thickness=thickness)[0]
    c1 = (x1y1[0], x1y1[1] - int(t_size[1] * 1.5)) \
      if x1y1[1] - int(t_size[1] * 1.5) >= 0 else (x1y1[0], x1y1[1])
    c2 = (x1y1[0] + t_size[0], x1y1[1]) \
      if x1y1[1] - int(t_size[1] * 1.5) >= 0 \
      else (x1y1[0] + t_size[0], x1y1[1] + int(t_size[1]*1.5))
    text_pos = (c1[0], c1[1]+t_size[1])
    img = cv2.rectangle(img, c1, c2, colors[int(classes[i])], -1)
    img = cv2.putText(img, class_names[int(classes[i])], text_pos,
            cv2.FONT_HERSHEY_SIMPLEX, thickness/3,
            (20, 20, 20), thickness=thickness, lineType=cv2.LINE_AA)
  return img


def test_img(src_img):
  yolo = YOLOv3(NUM_CLASSES)

  yolo.load_weights(WEIGHTS)
  print('weights loaded')

  class_names = [c.strip() for c in open(CLASSES).readlines()]
  print('classes loaded')
  img = cv2.imread(src_img)
  img_in = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
  img_in = tf.expand_dims(img_in, 0)
  img_in = transform_images(img_in, SIZE)
  boxes, scores, classes, nums = yolo.predict(img_in)
  img = draw_outputs(img, (boxes, scores, classes, nums), class_names, COLORS)
  cv2.imshow('output', img)
  cv2.waitKey(0)
  cv2.destroyAllWindows()


def main(num_classes, classes, weights, size, video, output, output_format, colors):
  yolo = YOLOv3(num_classes)

  yolo.load_weights(weights)
  print('weights loaded')

  class_names = [c.strip() for c in open(classes).readlines()]
  print('classes loaded')

  try:
    vid = cv2.VideoCapture(int(video))
  except:
    vid = cv2.VideoCapture(video)

  out = None

  if output:
    # by default VideoCapture returns float instead of int
    width = int(vid.get(cv2.CAP_PROP_FRAME_WIDTH))
    height = int(vid.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fps = int(vid.get(cv2.CAP_PROP_FPS))
    codec = cv2.VideoWriter_fourcc(*output_format)
    out = cv2.VideoWriter(output, codec, fps, (width, height))

  accum_time = 0
  curr_fps = 0
  fps = 'FPS: ??'
  prev_time = timer()

  while True:
    _, img = vid.read()

    if img is None:
      print('Empty Frame')
      continue

    img_in = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    img_in = tf.expand_dims(img_in, 0)
    img_in = transform_images(img_in, size)
    boxes, scores, classes, nums = yolo.predict(img_in)
    img = draw_outputs(img, (boxes, scores, classes, nums), class_names, colors)
      
    curr_time = timer()
    exec_time = curr_time - prev_time
    prev_time = curr_time
    accum_time = accum_time + exec_time
    curr_fps = curr_fps + 1
    if accum_time > 1:
      accum_time = accum_time - 1
      fps = 'FPS: ' + str(curr_fps)
      curr_fps = 0
    cv2.putText(img, text=fps, org=(3, 15), fontFace=cv2.FONT_HERSHEY_SIMPLEX,
                fontScale=0.50, color=(255, 255, 255), thickness=2)
    if output:
      out.write(img)
    cv2.imshow('output', img)
    if cv2.waitKey(1) == ord('q'):
      break

  cv2.destroyAllWindows()

if __name__ == '__main__':
  main(NUM_CLASSES, CLASSES, WEIGHTS, SIZE, VIDEO, OUTPUT, OUTPUT_FORMAT, COLORS)
