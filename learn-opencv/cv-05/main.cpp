/******************************************************************************************
 * Project Name: five
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-02-22
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-02-22
 *
 * Description: the 5th learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat image;
cv::Point originPoint;
cv::Rect selectedRect;
bool selectRegion = false;
int trackingFlag = 0;

static void onMouse(int event, int x, int y, int, void*) { // function to track the mouse events
	if(selectRegion) {
		selectedRect.x = MIN(x, originPoint.x);
		selectedRect.y = MIN(y, originPoint.y);
		selectedRect.width = std::abs(x - originPoint.x);
		selectedRect.height = std::abs(y - originPoint.y);
		selectedRect &= cv::Rect(0, 0, image.cols, image.rows);
	}

	switch(event) {
	case cv::EVENT_LBUTTONDOWN:
		originPoint = cv::Point(x,y);
		selectedRect = cv::Rect(x,y,0,0);
		selectRegion = true;
		break;

	case cv::EVENT_LBUTTONUP:
		selectRegion = false;
		if(selectedRect.width > 0 && selectedRect.height > 0) trackingFlag = -1;
		break;
	}
}

void coloredObjectTracker();
void camshiftTracker();
void harrisCornersTracker();
void goodFeaturesToTrack();
void franbackTracker();

/* *** Sparse optical flow tracking ***
	Lucas-Kanade
	http://cseweb.ucsd.edu/classes/sp02/cse252/lucaskanade81.pdf
***************************************/

int main(int argc, char const *argv[]) {
	if (argc != 2) { // parameter error
		//std::cout << "Usage: ./Testing <Image Path>" << std::endl;
		return -1;
	}
	//cv::namedWindow("Five"); // create a window
	cv::Mat input, result;
	input = cv::imread(argv[1]);

	cv::imshow("Input", input);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

void coloredObjectTracker() {
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened()) return;
	cv::Mat frame, hsvImage, mask, result;
	float scalingFactor = 0.75;

	while(true) {
		result = cv::Scalar(0, 0, 0);
		cap >> frame;
		if(frame.empty()) break;
		cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
		cv::cvtColor(frame, hsvImage, cv::COLOR_BGR2HSV); // convert to HSV colorspace

		cv::Scalar lowerLimit = cv::Scalar(60, 100, 100);
		cv::Scalar upperLimit = cv::Scalar(180, 255, 255);
		// threshold the HSV image to get only blue color
		cv::inRange(hsvImage, lowerLimit, upperLimit, mask);
		cv::bitwise_and(frame, frame, result, mask = mask);
		cv::medianBlur(result, result, 5); // run median filter on the output to smoothen it

		cv::imshow("Input", frame);
		cv::imshow("Result", result);

		char c = cv::waitKey(30);
		if (c == 27) break;
	}
	cap.release();
}

void camshiftTracker() {
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened()) return;
	cv::Rect trackingRect;

	
	float hueRanges[] = {0, 180}; // range of values for the 'H'(Hue) channel in HSV
	const float* histRanges = hueRanges;
	int minSaturation = 40; //min value for the 'S'(Saturation) channel
	int minValue = 20, maxValue = 245; // min and max values for the 'V'(Value) channel
	int histSize = 8; // size of the histogram bin

	cv::namedWindow("CAMShift Tracker", 0);
	cv::setMouseCallback("CAMShift Tracker", onMouse, 0);

	cv::Mat frame, hsvImage, hueImage, mask, hist, backproj;
	float scalingFactor = 0.75;

	while(true) {
		cap >> frame;
		if(frame.empty()) break;
		cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
		frame.copyTo(image); //clone the input frame
		cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

		if(trackingFlag) {
			// check for all the values in 'hsvimage' that are within the specified range and put the result in 'mask'
			cv::inRange(hsvImage, cv::Scalar(0, minSaturation, minValue), cv::Scalar(180, 256, maxValue), mask);

			int channels[] = {0, 0}; // mix the specified channels
			hueImage.create(hsvImage.size(), hsvImage.depth());
			cv::mixChannels(&hsvImage, 1, &hueImage, 1, channels, 1);

			if(trackingFlag < 0) {
				// create images based on selected ROI
				cv::Mat imageROI(hueImage, selectedRect), maskroi(mask, selectedRect);
				// compute the histogram and normalize it
				cv::calcHist(&imageROI, 1, 0, maskroi, hist, 1, &histSize, &histRanges);
				cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);

				trackingRect = selectedRect;
				trackingFlag = 1;
			}
		
			cv::calcBackProject(&hueImage, 1, 0, hist, backproj, &histRanges); // compute the histogram back projection
			backproj &= mask;
			cv::RotatedRect rotatedTrackingRect = cv::CamShift(backproj, trackingRect, 
				cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1));

			if(trackingRect.area() <= 1) { // if area is too small
				// Use an offset value to make sure the trackingRect has a minimum size
				int cols = backproj.cols, rows = backproj.rows;
				int offset = MIN(rows, cols) + 1;
				trackingRect = cv::Rect(trackingRect.x - offset, trackingRect.y - offset, 
					trackingRect.x + offset, trackingRect.y + offset) & cv::Rect(0, 0, cols, rows);
			}
			cv::ellipse(image, rotatedTrackingRect, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
		}
		
		if(selectRegion && selectedRect.width > 0 && selectedRect.height > 0) { // apply the 'negative' effect on the selected ROI
			cv::Mat imageROI(image, selectedRect);
			cv::bitwise_not(imageROI, imageROI);
		}

		cv::imshow("CAMShift Tracker", image);
		char c = cv::waitKey(30);
		if (c == 27) break;
	}
}

void harrisCornersTracker() {
	// http://www.bmva.org/bmvc/1988/avc-88-023.pdf
	int blockSize = 1; // must small than 2
	int apertureSize = 5, thresh = 200;
	double k = 0.04;
	cv::RNG rng(12345);
	cv::Mat frame, frameGray;
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened()) return;

	float scalingFactor = 0.75;
	cv::Mat dst, dst_norm, dst_norm_scaled;

	while(true){
		cap >> frame;
		cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
		dst = cv::Mat::zeros(frame.size(), CV_32FC1);
		cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY );
		cv::cornerHarris(frameGray, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
		cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
		cv::convertScaleAbs(dst_norm, dst_norm_scaled);

		
		for(auto j = 0; j < dst_norm.rows ; j++) { // draw a circle around corners
			for(auto i = 0; i < dst_norm.cols; i++) {
				if((int)dst_norm.at<float>(j,i) > thresh) {
					cv::circle(frame, cv::Point(i, j), 8, cv::Scalar(0, 255, 0), 2, 8, 0);
				}
			}
		}

		imshow("Harris Corner Detector", frame);
		char c = cv::waitKey(30);
		if (c == 27) break;
	}
	cap.release();
}

void goodFeaturesToTrack() {
	// http://www.ai.mit.edu/courses/6.891/handouts/shi94good.pdf
	int numCorners = 1; // must big than 1
	cv::RNG rng(12345);
	cv::Mat frame, frameGray;
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened())return;
	float scalingFactor = 0.75;

	while(true) {
		cap >> frame;
		cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
		cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY );
		std::vector<cv::Point2f> corners; // initialize the parameters for Shi-Tomasi algorithm
		double qualityThreshold = 0.02, minDist = 15, k = 0.07;
		int blockSize = 5;
		bool useHarrisDetector = false;
		cv::Mat frameCopy;
		frameCopy = frame.clone();
		// corner detection
		cv::goodFeaturesToTrack(frameGray, corners, numCorners, 
			qualityThreshold, minDist, cv::Mat(), blockSize, useHarrisDetector, k);
		int radius = 8, thickness = 2, lineType = 8; // radius of the cirles, thickness of the circles
		for(auto i = 0; i < corners.size(); i++){ // draw the detected corners using circles
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			cv::circle(frameCopy, corners[i], radius, color, thickness, lineType, 0);
		}

		cv::imshow("Feature points", frameCopy);
		char c = cv::waitKey(30);
		if (c == 27) break;
	}
	cap.release();
}

void drawOpticalFlow(const cv::Mat& flowImage, cv::Mat& flowImageGray) { // compute the optical flow map
	int stepSize = 16;
	cv::Scalar color = cv::Scalar(0, 255, 0);
	// draw the uniform grid of points on the input image along with the motion vectors
	for(auto y = 0; y < flowImageGray.rows; y += stepSize) {
		for(auto x = 0; x < flowImageGray.cols; x += stepSize) {
			// circles to indicate the uniform grid of points
			int radius = 2, thickness = -1;
			cv::circle(flowImageGray, cv::Point(x, y), radius, color, thickness);
			// lines to indicate the motion vectors
			cv::Point2f pt = flowImage.at<cv::Point2f>(y, x);
			cv::line(flowImageGray, cv::Point(x, y), cv::Point(cvRound(x + pt.x), cvRound(y + pt.y)), color);
		}
	}
}


void franbackTracker() {
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened())return;
	cv::Mat curGray, prevGray, flowImage, flowImageGray, frame;
	float scalingFactor = 0.75;

	while(true) {
		cap >> frame;
		if(frame.empty()) break;

		cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
		cv::cvtColor(frame, curGray, cv::COLOR_BGR2GRAY);
		if(prevGray.data) {
			// initialize parameters for the optical flow algorithm
			float pyrScale = 0.5, stdDeviation = 1.2;
			int numLevels = 3, windowSize = 15, numIterations = 3, neighborhoodSize = 5;

			cv::calcOpticalFlowFarneback(prevGray, curGray, // calculate optical flow map using Farneback algorithm
				flowImage, pyrScale, numLevels, windowSize, numIterations, 
				neighborhoodSize, stdDeviation, cv::OPTFLOW_USE_INITIAL_FLOW);
			cv::cvtColor(prevGray, flowImageGray, cv::COLOR_GRAY2BGR);
			drawOpticalFlow(flowImage, flowImageGray);

			cv::imshow("Optical Flow", flowImageGray);
		}

	char c = cv::waitKey(30);
	if(c == 27) break;
	std::swap(prevGray, curGray); // swap previous image with the current image
	}
	cap.release();
}