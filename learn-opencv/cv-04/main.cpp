/******************************************************************************************
 * Project Name: four
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-02-21
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-02-22
 *
 * Description: the 4th learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

void backgroundSubtraction();
void frameDifferencing();
cv::Mat performDilation(cv::Mat, int, int);
cv::Mat performErosion(cv::Mat, int, int);
cv::Mat performOpening(cv::Mat, int, int);
cv::Mat performClosing(cv::Mat, int, int);
cv::Mat performTopHat(cv::Mat, int, int);
cv::Mat performBlackHat(cv::Mat, int, int);
cv::Mat performMorphologicalGradient(cv::Mat, int, int);


int main(int argc, char const *argv[]) {
	if (argc != 3) { // parameter error
		//std::cout << "Usage: ./Testing <Image Path> <Value for Dilation/Erosion>" << std::endl;
		return -1;
	}
	//cv::namedWindow("Four"); // create a window
	cv::Mat input, result;
	input = cv::imread(argv[1]);
	int dilationSize;
	std::istringstream iss(argv[2]);
	iss >> dilationSize;
	int dilationElement = 0;

	result = performErosion(input, dilationElement, dilationSize);
	cv::imshow("Input", input);
	cv::imshow("Dilation/Erosion", result);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

void backgroundSubtraction() {
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened()) return;
	cv::Mat frame, fgMaskMOG2;
	cv::Ptr<cv::BackgroundSubtractor> pMOG2;

	pMOG2 = cv::createBackgroundSubtractorMOG2();
	float scalingFactor = 0.75;

	while(true) {
		cap >> frame;
		cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
		pMOG2->apply(frame, fgMaskMOG2);
		cv::imshow("Frame", frame);
		cv::imshow("FG Mask MOG 2", fgMaskMOG2);
		char c = cv::waitKey(30);
		if(c == 27) break;
	}
	cap.release();
}

cv::Mat frameDiff(cv::Mat prevFrame, cv::Mat curFrame, cv::Mat nextFrame) {
	cv::Mat diff1, diff2, output;
	// compute absolute difference between current frame and the next frame
	cv::absdiff(nextFrame, curFrame, diff1);
	// compute absolute difference between current frame and the previous frame
	cv::absdiff(curFrame, prevFrame, diff2);
	bitwise_and(diff1, diff2, output); // bitwise "AND" operation between the above two diff images
	return output;
}

cv::Mat getFrame(cv::VideoCapture cap, float scalingFactor) {
	cv::Mat frame, output;
	cap >> frame;
	cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
	cv::cvtColor(frame, output, cv::COLOR_BGR2GRAY);
	return output;
}

void frameDifferencing() {
	cv::Mat frame, prevFrame, curFrame, nextFrame;
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened()) return;
	float scalingFactor = 0.75;

	prevFrame = getFrame(cap, scalingFactor);
	curFrame = getFrame(cap, scalingFactor);
	nextFrame = getFrame(cap, scalingFactor);

	while(true) { // show the object movement
		cv::imshow("Object Movement", frameDiff(prevFrame, curFrame, nextFrame));
		prevFrame = curFrame; // update the variables and grab the next frame
		curFrame = nextFrame;
		nextFrame = getFrame(cap, scalingFactor);

		char c = cv::waitKey(30);
		if(c == 27) break;
	}
	cap.release();
}

cv::Mat performDilation(cv::Mat input, int dilationElement, int dilationSize) { // function to perform dilation
	cv::Mat result;
	int dilationType;
	if(dilationElement == 0) dilationType = cv::MORPH_RECT;
	else if(dilationElement == 1) dilationType = cv::MORPH_CROSS;
	else if(dilationElement == 2) dilationType = cv::MORPH_ELLIPSE;

	cv::Mat element = cv::getStructuringElement(dilationType, // create the structuring element for dilation
		cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1), cv::Point(dilationSize, dilationSize));
	cv::dilate(input, result, element); // dilate the image using the structuring element
	
	return result;
}

cv::Mat performErosion(cv::Mat input, int erosionElement, int erosionSize) { // function to perform erosion
	cv::Mat result;
	int erosionType;
	if(erosionElement == 0) erosionType = cv::MORPH_RECT;
	else if(erosionElement == 1) erosionType = cv::MORPH_CROSS; 
	else if(erosionElement == 2) erosionType = cv::MORPH_ELLIPSE;

	cv::Mat element = cv::getStructuringElement(erosionType, 
		cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1), cv::Point(erosionSize, erosionSize));
	cv::erode(input, result, element);
	
	return result;
}

cv::Mat performOpening(cv::Mat input, int morphologyElement, int morphologySize) { // perform morphological opening
	cv::Mat result, temp;
	int morphologyType;
	if(morphologyElement == 0) morphologyType = cv::MORPH_RECT;
	else if(morphologyElement == 1) morphologyType = cv::MORPH_CROSS;
	else if(morphologyElement == 2) morphologyType = cv::MORPH_ELLIPSE;
	// erosion first, then dilation
	cv::Mat element = cv::getStructuringElement(morphologyType, 
		cv::Size(2 * morphologySize + 1, 2 * morphologySize + 1), cv::Point(morphologySize, morphologySize));
	cv::erode(input, temp, element);
	cv::dilate(temp, result, element);

	return result;
}

cv::Mat performClosing(cv::Mat input, int morphologyElement, int morphologySize) { // perform morphological closing
	cv::Mat result, temp;
	int morphologyType;
	if(morphologyElement == 0) morphologyType = cv::MORPH_RECT;
	else if(morphologyElement == 1) morphologyType = cv::MORPH_CROSS;
	else if(morphologyElement == 2) morphologyType = cv::MORPH_ELLIPSE;
	// dilation first, then erosion
	cv::Mat element = cv::getStructuringElement(morphologyType, 
		cv::Size(2 * morphologySize + 1, 2 * morphologySize + 1), cv::Point(morphologySize, morphologySize));
	cv::dilate(input, temp, element);
	cv::erode(temp, result, element);

	return result;
}

cv::Mat performTopHat(cv::Mat input, int morphologyElement, int morphologySize) { // perform top hat operation
	cv::Mat result;
	int morphologyType;
	if(morphologyElement == 0) morphologyType = cv::MORPH_RECT;
	else if(morphologyElement == 1) morphologyType = cv::MORPH_CROSS;
	else if(morphologyElement == 2) morphologyType = cv::MORPH_ELLIPSE;
	// difference between input image and its morphological opening
	cv::Mat element = cv::getStructuringElement(morphologyType, 
		cv::Size(2 * morphologySize + 1, 2 * morphologySize + 1), cv::Point(morphologySize, morphologySize));
	result = input - performOpening(input, morphologyElement, morphologySize);

	return result;
}

cv::Mat performBlackHat(cv::Mat input, int morphologyElement, int morphologySize) { // perform black hat operation
	cv::Mat result;
	int morphologyType;
	if(morphologyElement == 0) morphologyType = cv::MORPH_RECT;
	else if(morphologyElement == 1) morphologyType = cv::MORPH_CROSS;
	else if(morphologyElement == 2) morphologyType = cv::MORPH_ELLIPSE;
	// difference between input image and its morphological closing
	cv::Mat element = cv::getStructuringElement(morphologyType, 
		cv::Size(2 * morphologySize + 1, 2 * morphologySize + 1), cv::Point(morphologySize, morphologySize));
	result = performClosing(input, morphologyElement, morphologySize) - input;

	return result;
}

cv::Mat performMorphologicalGradient(cv::Mat input, int morphologyElement, int morphologySize) { // perform morphological gradient
	cv::Mat result, temp1, temp2;
	int morphologyType;
	if(morphologyElement == 0)morphologyType = cv::MORPH_RECT;
	else if(morphologyElement == 1) morphologyType = cv::MORPH_CROSS;
	else if(morphologyElement == 2) morphologyType = cv::MORPH_ELLIPSE;
	// difference between erosion and dilation
	cv::Mat element = cv::getStructuringElement(morphologyType, 
		cv::Size(2 * morphologySize + 1, 2 * morphologySize + 1), cv::Point(morphologySize, morphologySize));
	cv::dilate(input, temp1, element);
	cv::erode(input, temp2, element);

	return temp1 - temp2;
}