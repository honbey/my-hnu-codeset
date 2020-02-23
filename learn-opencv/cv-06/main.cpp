/******************************************************************************************
 * Project Name: six
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-02-23
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-02-23
 *
 * Description: the 6th learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

float confThreshold = 0.5, nmsThreshold = 0.4; // confidence threshold, non-maximum suppression threshold
int inpWidth = 416, inpHeight = 416; // initialize the parameters
std::vector<std::string> classes;

void drawPred(int, float, int, int, int, int, cv::Mat&);
void postprocess(cv::Mat&, const std::vector<cv::Mat>&);
std::vector<cv::String> getOutputsNames(const cv::dnn::Net&);

int main(int argc, char const *argv[]) {
	if (argc != 2) { // parameter error
		//std::cout << "Usage: ./Testing <Image Path>" << std::endl;
		return -1;
	}
	//cv::namedWindow("Five"); // create a window
	cv::Mat input, result, blob;
	input = cv::imread(argv[1]);
	if (input.empty()) return -2;

	std::string classesFile = "../../../data/ml/coco.names", line; // load names of classes
	std::ifstream ifs(classesFile.c_str());
	while (getline(ifs, line)) classes.push_back(line);
	
	cv::String modelConfiguration = "../../../data/ml/yolov3.cfg"; // give the configuration and weight files for the model
	cv::String modelWeights = "../../../data/ml/yolov3.weights";

	cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelConfiguration, modelWeights); // load the network
	net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	// create a 4D blob from a frame
	cv::dnn::blobFromImage(input, blob, 1 / 255.0, 
		cv::Size(inpWidth, inpHeight), cv::Scalar(0, 0, 0), true, false);
	net.setInput(blob); //sets the input to the network
	std::vector<cv::Mat> outs; // runs the forward pass to get output of the output layers
	net.forward(outs, getOutputsNames(net));

	postprocess(input, outs); // remove the bounding boxes with low confidence
	
	std::vector<double> layersTimes; // put efficiency information
	double freq = cv::getTickFrequency() / 1000, t = net.getPerfProfile(layersTimes) / freq;
	std::string label = cv::format("Inference time for compute the image : %.2f ms", t);
	std::cout << label << std::endl;

	cv::imshow("Deep Learning", input);
	//cv::imwrite("result.jpg", input);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

// draw the predicted bounding box
void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) {
	// draw a rectangle displaying the bounding box
	cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(255, 255, 255), 1);
	// get the label for the class name and its confidence
	std::string conf_label = cv::format("%.2f", conf);
	std::string label="";
	if (!classes.empty()) label = classes[classId] + ":" + conf_label;

	int baseLine; // display the label
	cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
	top = cv::max(top, labelSize.height);
	cv::rectangle(frame, cv::Point(left, top - labelSize.height), 
		cv::Point(left + labelSize.width, top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
	cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1, cv::LINE_AA);
}

// remove the bounding boxes with low confidence using non-maxima suppression
void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs) {
	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;

	for(auto i = 0; i < outs.size(); i++) {
		// Scan through all the bounding boxes output from the network and keep only the
		// ones with high confidence scores. Assign the box's class label as the class
		// with the highest score for the box.
		float* data = (float*)outs[i].data;
		for(auto j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
			cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			cv::Point classIdPoint;
			double confidence; // get the value and location of the maximum score
			cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold) {
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(cv::Rect(left, top, width, height));
			}
		}
	}
	// perform non maximum suppression to eliminate redundant overlapping boxes with lower confidences
	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	for (auto i = 0; i < indices.size(); i++) {
		int idx = indices[i];
		cv::Rect box = boxes[idx];
		drawPred(classIds[idx], confidences[idx], box.x, box.y,
			box.x + box.width, box.y + box.height, frame);
	}
}

std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net) { // get the names of the output layers
	static std::vector<cv::String> names;
	if(names.empty()) {
		std::vector<int> outLayers = net.getUnconnectedOutLayers(); // get the indices of the output layers
		std::vector<cv::String> layersNames = net.getLayerNames(); // get the names of all the layers in the network
		names.resize(outLayers.size()); // get the names of the output layers in names
		for (auto i = 0; i < outLayers.size(); i++) {
			names[i] = layersNames[outLayers[i] - 1];
		}
	}
	return names;
}