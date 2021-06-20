import numpy as np
from yolo import YOLOv3

NUM_CLASSES = 3
WEIGHTS = './model_data/yolov3_20000.weights'
OUTPUT = './model_data/yolov3.tf'


def load_darknet_weights(model, weights_file):
  file = open(weights_file, 'rb')
  major, minor, revision, seen, _ = np.fromfile(file, dtype=np.int32, count=5)
  layers = [
    'yolo_darknet',
    'yolo_conv_0',
    'yolo_output_0',
    'yolo_conv_1',
    'yolo_output_1',
    'yolo_conv_2',
    'yolo_output_2',
  ]

  for layer_name in layers:
    sub_model = model.get_layer(layer_name)
    for i, layer in enumerate(sub_model.layers):
      if not layer.name.startswith('conv2d'):
        continue
      batch_norm = None
      if i + 1 < len(sub_model.layers) and \
            sub_model.layers[i + 1].name.startswith('batch_norm'):
        batch_norm = sub_model.layers[i + 1]

      print("{}/{} {}".format(
        sub_model.name, layer.name, 'bn' if batch_norm else 'bias'))

      filters = layer.filters
      size = layer.kernel_size[0]
      in_dim = layer.get_input_shape_at(0)[-1]

      if batch_norm is None:
        conv_bias = np.fromfile(file, dtype=np.float32, count=filters)
      else:
        bn_weights = np.fromfile(
          file, dtype=np.float32, count=4 * filters)
        bn_weights = bn_weights.reshape((4, filters))[[1, 0, 2, 3]]

      conv_shape = (filters, in_dim, size, size)
      conv_weights = np.fromfile(
        file, dtype=np.float32, count=np.product(conv_shape))
      conv_weights = conv_weights.reshape(
          conv_shape).transpose([2, 3, 1, 0])

      if batch_norm is None:
        layer.set_weights([conv_weights, conv_bias])
      else:
        layer.set_weights([conv_weights])
        batch_norm.set_weights(bn_weights)

  assert len(file.read()) == 0, 'failed to read all data'
  file.close()


def main(num_classes, weights, output):
  yolo = YOLOv3(num_classes)
  yolo.summary()
  print('model created')

  load_darknet_weights(yolo, weights)
  print('weights loaded')

  yolo.save_weights(output)
  print('weights saved')


if __name__ == '__main__':
  main(NUM_CLASSES, WEIGHTS, OUTPUT)