/******************************************************************************************
 * Project Name: four
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-02-21
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-02-21
 *
 * Description: the 4th learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

void backgroundSubtraction();
cv::Mat performDilation(cv::Mat, int, int);
cv::Mat performErosion(cv::Mat, int, int);

int main(int argc, char const *argv[]) {
	if (argc != 3) { // parameter error
		//std::cout << "Usage: ./Testing <Image Path>" << std::endl;
		return -1;
	}
	//cv::namedWindow("Four"); // create a window
	cv::Mat inputImage, outputImage;
	inputImage = cv::imread(argv[1]);
	int dilationSize;
	std::istringstream iss(argv[2]);
	iss >> dilationSize;
	int dilationElement = 0;

	cv::namedWindow("Input image", WINDOW_AUTOSIZE);
	cv::namedWindow("Output image after dilation", WINDOW_AUTOSIZE);
	outputImage = performDilation(inputImage, dilationElement, dilationSize);
	cv::imshow("Input image", inputImage);
	cv::imshow("Output image after dilation", outputImage);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

void backgroundSubtraction() {
	cv::VideoCapture cap;
	cap.open("");
	if(!cap.isOpened()) return -1;
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

cv::Mat performDilation(cv::Mat inputImage, int dilationElement, int dilationSize) { // function to perform dilation
	cv::Mat outputImage;
	int dilationType;
	if(dilationElement == 0) dilationType = MORPH_RECT;
	else if(dilationElement == 1) dilationType = MORPH_CROSS;
	else if(dilationElement == 2) dilationType = MORPH_ELLIPSE;

	cv::Mat element = cv::getStructuringElement(dilationType, // create the structuring element for dilation
		cv::Size(2 * dilationSize + 1, 2 * dilationSize + 1), cv::Point(dilationSize, dilationSize));
	cv::dilate(inputImage, outputImage, element); // dilate the image using the structuring element
	
	return outputImage;
}

cv::Mat performErosion(cv::Mat inputImage, int erosionElement, int erosionSize) { // function to perform erosion
	cv::Mat outputImage;
	int erosionType;
	if(erosionElement == 0) erosionType = MORPH_RECT;
	else if(erosionElement == 1) erosionType = MORPH_CROSS; 
	else if(erosionElement == 2) erosionType = MORPH_ELLIPSE;

	cv::Mat element = cv::getStructuringElement(erosionType, 
		cv::Size(2 * erosionSize + 1, 2 * erosionSize + 1), cv::Point(erosionSize, erosionSize));
	cv::erode(inputImage, outputImage, element);
	
	return outputImage;
}