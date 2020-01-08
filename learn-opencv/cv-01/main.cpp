/******************************************************************************************
 * Project Name: one
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-01-07
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-01-07
 *
 * Description: the 1st learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

void showImage(cv::Mat); // show image
void imageProc(cv::Mat); // simple image processing
void imagePaint(cv::Mat); // paint on image
void fileOP(); // file operation
void showVideo(); // play Video

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		std::cout << "Usage: " << std::endl;
		return -1;
	}

	cv::namedWindow("One", cv::WINDOW_GUI_EXPANDED); // create a window
	cv::Mat img = cv::imread(argv[1]); // read a image
	if (!img.data) {
		std::cout << "error: No image data" << std::endl;
		return -2;
	}

	//showImage(img);
	//imageProc(img);
	//imagePaint(img);
	//fileOP();
	showVideo();

	cv::waitKey(0); // press any key to exit
	cv::destroyAllWindows(); // close all windows
	return 0;
}

void showImage(cv::Mat img) {
	cv::imshow("One", img); // show image
	cv::waitKey(0);
}

void imageProc(cv::Mat img) {
	cv::Mat img_a, img_b;
	cv::imshow("One", img); // show row image
	cv::namedWindow("Smooth", cv::WINDOW_AUTOSIZE);
	cv::GaussianBlur(img, img_a, cv::Size(5, 5), 3, 3);
	cv::imshow("Smooth", img_a);
	cv::namedWindow("Ashing", cv::WINDOW_AUTOSIZE);
	cv::cvtColor(img, img_b, cv::COLOR_BGR2GRAY);
	cv::imshow("Ashing", img_b);
	cv::namedWindow("Split",cv::WINDOW_AUTOSIZE);
	cv::pyrDown(img, img_b);
	cv::imshow("Split", img_b);
	cv::namedWindow("Canny", cv::WINDOW_AUTOSIZE);
	cv::Canny(img, img_b, 10, 100, 3, true);
	cv::imshow("Canny", img_b);
	cv::namedWindow("Canny_Smooth", cv::WINDOW_AUTOSIZE);
	cv::Canny(img_a, img_b, 10, 100, 3, true);
	cv::imshow("Canny_Smooth", img_b);	
	cv::waitKey(0);
}

void imagePaint(cv::Mat img) {
	cv::circle(img, cv::Point2i(200, 200), 30, 
		cv::COLOR_GRAY2BGR, 6, 8, 0); // paint a circle which centre is (200, 200), radius is 90 with pixel width of 6
	cv::ellipse(img, cv::Point2i(200, 200), cv::Size2i(100, 200), 0, 0, 360, 
		cv::COLOR_GRAY2BGR, 3, 8, 0); // paint a ellipse
	cv::line(img, cv::Point2i(100, 100), cv::Point2i(360, 300), 
		cv::COLOR_GRAY2BGR, 8, 0); // line 
	cv::rectangle(img, cv::Point2i(66, 50), cv::Point2i(123, 345), 
		cv::Scalar(100, 100, 100), 8, 0); // rectangle
	cv::putText(img, "Hello", cv::Point2i(60, 120), cv::FONT_HERSHEY_SCRIPT_COMPLEX, 5.0, 
		cv::Scalar(255, 0, 0), 5, 8, 0); // display "Hello"
	cv::putText(img, "Test", cv::Point2i(120, 240), cv::FONT_HERSHEY_SIMPLEX, 5.0, 
		cv::Scalar(0, 255, 0), 5, 8, 0); // display "Test" 
	cv::putText(img, "Write", cv::Point2i(60, 360), cv::FONT_HERSHEY_PLAIN, 5.0, 
		cv::Scalar(0, 0, 255), 5, 8, 0); // display "Write", 3 channels (BGR)
	cv::imshow("One", img); // show
	
	cv::Size sH = cv::getTextSize("H", cv::FONT_HERSHEY_SCRIPT_COMPLEX, 5.0, 5, NULL);
	cv::Size sW = cv::getTextSize("W", cv::FONT_HERSHEY_PLAIN, 5.0, 5, NULL);
	std::cout << "(" << sH.height << "," << sH.width << ")" << std::endl;
	std::cout << "(" << sW.height << "," << sW.width << ")" << std::endl;

	cv::waitKey(0);	
}

void fileOP() {
	cv::FileStorage fr("test.xml", cv::FileStorage::READ);
	cv::FileStorage fw("test.yml", cv::FileStorage::WRITE);
	int imagewidth = 5;
	int imageheight = 10;
	fw << "imagewidth" << imagewidth;
	fw << "imageheight" << imageheight;

	cv::Mat m1 = cv::Mat::eye(3, 3, CV_8U);
	cv::Mat m2 = cv::Mat::ones(3, 3, CV_8U);
	cv::Mat resultMat = (m1 + 1).mul(m1 + 2);
	fw << "resultMat" << resultMat;

	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) << 1000, 0, 320, 0, 1000, 240, 0, 0, 1);
	cv::Mat distCoeffs = (cv::Mat_<double>(5, 1) << 0.1, 0.01, -0.001, 0, 0);
	fw << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;

	fr.release();
	fw.release();
}

void showVideo() {
	cv::VideoCapture cap; // a video capture
	cap.open("../bike.avi");
	if(!cap.isOpened()) {
		std::cout << "can't open the file!" << std::endl;
		return;
	}
	
	cv::Mat frame;
	while(true) {
		cap >> frame;
		if(frame.empty()) break;
		cv::imshow("Show Video", frame);
		char c = cv::waitKey(1000/15);
		if(c == 27) break;
	}
}
