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

void imageProc(cv::Mat); // simple image processing
void imagePaint(cv::Mat); // paint on image
void fileOP(); // file operation
void showVideo(); // play Video
void showHist(cv::Mat); // show the histogram
void equalized(cv::Mat);
void lomo(cv::Mat);
void cartoon(cv::Mat);
void understandMat(cv::Mat); // understand the cv::Mat
void imageROI(cv::Mat); // Region of Interest 

static void onMouse(int, int, int, int, void*); // mouse callback function
static void onChange(int, void*); // slider bar callback function

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		std::cout << "Usage: " << std::endl;
		return -1;
	}

	cv::namedWindow("One", cv::WINDOW_GUI_EXPANDED); // create a window
	cv::Mat img = cv::imread(argv[1]); // read a image
	if (!img.data) { // image is empty or can't open it
		std::cout << "error: No image data" << std::endl;
		return -2;
	}
	cv::setMouseCallback("One", onMouse, reinterpret_cast<void*>(&img)); // Cpp cast
	int blurFlag = 15;
	cv::createTrackbar("Blur Bar", "One", &blurFlag, 30, onChange, &img);
	cv::imshow("One", img); // show image
	//imageProc(img);
	//imagePaint(img);
	//fileOP();
	//showVideo();
	//showHist(img);
	//equalized(img);
	//lomo(img);
	//cartoon(img);
	understandMat(img);
	imageROI(img);

	cv::waitKey(0); // press any key to exit
	cv::destroyAllWindows(); // close all windows
	return 0;
}

static void onMouse(int event, int x, int y, int flags, void *param) {
	flags = 1;
	if (!flags) return; // avoid warning
	cv::Mat *im = reinterpret_cast<cv::Mat*>(param);
	switch (event) {
		case cv::EVENT_LBUTTONDOWN: // left button click
			std::cout << "at ("
					<< x << ", " << y
					<< ") value is: BGR("
					<< static_cast<int>(im->at<cv::Vec3b>(cv::Point(x, y))[0]) << ", "
					<< static_cast<int>(im->at<cv::Vec3b>(cv::Point(x, y))[1]) << ", "
					<< static_cast<int>(im->at<cv::Vec3b>(cv::Point(x, y))[2]) << ")."
					<< std::endl;
		break;
	}
}

static void onChange(int pos, void *uesrInput) {
	if (pos <= 0) return;
	cv::Mat imgBlur;
	cv::Mat *imgPtr = static_cast<cv::Mat*>(uesrInput); // Cpp cast
	cv::blur(*imgPtr, imgBlur, cv::Size(pos, pos)); // blur image by position
	imshow("One", imgBlur);
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
}

void fileOP() {
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
	cv::FileStorage fr("test.yml", cv::FileStorage::READ); // only read file

	fr.release();
	fw.release();
}

void showVideo() {
	cv::VideoCapture cap; // a video capture
	cap.open("../bike.avi");
	if(!cap.isOpened()) { // can't open the capture
		std::cout << "can't open the file!" << std::endl;
		return;
	}
	cv::Mat frame; // create a cv::Mat to accept each frame
	while(true) {
		cap >> frame;
		if(frame.empty()) break;
		cv::imshow("Show Video", frame);
		char c = cv::waitKey(1000/15);
		if(c == 27) break;
	}
}

void showHist(cv::Mat img) {
	std::vector<cv::Mat> bgr;
	cv::split(img, bgr);
	int numRange = 256; // possible number of values is 256 / [0..255]
	float range[] = {0, 256}; // set range (B, G, R)
	const float *histRange = {range}; // pointer to array

	cv::Mat b_hist, g_hist, r_hist; // draw and calculate for each channel
	cv::calcHist(&bgr[0], 1, 0, cv::Mat(), b_hist, 1, &numRange, &histRange);
	cv::calcHist(&bgr[1], 1, 0, cv::Mat(), g_hist, 1, &numRange, &histRange);
	cv::calcHist(&bgr[2], 1, 0, cv::Mat(), r_hist, 1, &numRange, &histRange);
	int width = 512;
	int heigth = 300;
	// create an image based on gray and normalized histogram from zero to image's BGR value
	cv::Mat histImg(heigth, width, CV_8UC3, cv::Scalar(20, 20, 20));
	cv::normalize(b_hist, b_hist, 0, heigth, cv::NORM_MINMAX);
	cv::normalize(g_hist, g_hist, 0, heigth, cv::NORM_MINMAX);
	cv::normalize(r_hist, r_hist, 0, heigth, cv::NORM_MINMAX);

	int binStep = cvRound(static_cast<float>(width) / static_cast<float>(numRange));
	for (int i = 1; i < numRange; i++) { // draw a horizontal line segment from i - 1 to i
		cv::line(histImg,
				 cv::Point(binStep * (i - 1), heigth - cvRound(b_hist.at<float>(i-1))),
				 cv::Point(binStep * i, heigth - cvRound(b_hist.at<float>(i))),
				 cv::Scalar(255, 0, 0));
		cv::line(histImg,
				 cv::Point(binStep * (i - 1), heigth - cvRound(g_hist.at<float>(i-1))),
				 cv::Point(binStep * i, heigth - cvRound(g_hist.at<float>(i))),
				 cv::Scalar(0, 255, 0));
		cv::line(histImg,
				 cv::Point(binStep * (i - 1), heigth - cvRound(r_hist.at<float>(i-1))),
				 cv::Point(binStep * i, heigth - cvRound(r_hist.at<float>(i))),
				 cv::Scalar(0, 0, 255));
	}
	cv::imshow("Histogram", histImg); // show histogram
}

void equalized(cv::Mat img) { // histogram equalization
	cv::Mat result, ycrcb; // result is the final result, ycrcb is a variable used for conversion
	cv::cvtColor(img, ycrcb, cv::COLOR_BGR2YCrCb); // BGR to YCrCb
	std::vector<cv::Mat> channels;
	cv::split(ycrcb, channels); // split image channel
	cv::equalizeHist(channels[0], channels[0]); // only equalize Y channel
	cv::merge(channels, ycrcb); // merge result and channel
	cv::cvtColor(ycrcb, result, cv::COLOR_YCrCb2BGR); // YCrCb to BGR
	cv::imshow("equalized", result); // show image equalized
}

void lomo(cv::Mat img) { // LOMO effect / retro effect of dimmed image
	cv::Mat result;
	const double exponential_e = std::exp(1.0); // Euler's Number
	cv::Mat lut(1, 256, CV_8UC1); // create a mapping table with 256 elements
	for (int i = 0; i < 256; i++) { // procesing R channel by curve change
		float x = static_cast<float>(i) / 256.0;
		// 1 / (1 + e^(-((x-0.5)/S)))
		lut.at<uchar>(i) = cvRound(256 * ( 1 / (1 + pow(exponential_e, -((x - 0.5) / 0.1)))));
	}
	std::vector<cv::Mat> bgr; 
	cv::split(img, bgr); // split image channel, only process R channel
	cv::LUT(bgr[2], lut, bgr[2]); // Lasest Used Table, use 'lut'
	cv::merge(bgr, result); // merge result
	// create a dull image
	cv::Mat halo(img.rows, img.cols, CV_32FC3, cv::Scalar(0.3, 0.3, 0.3));
	cv::circle(halo, cv::Point(img.cols / 2, img.rows / 2), img.cols / 3, cv::Scalar(1, 1, 1), -1);
	cv::blur(halo, halo, cv::Size(img.cols / 3, img.rows / 3)); // blur filter function makes the image more blurred
	// we need blurred image, merge two images
	cv::Mat resultf; // convert 'result' to float type
	result.convertTo(resultf, CV_32FC3);
	cv::multiply(resultf, halo, resultf);
	resultf.convertTo(result, CV_8UC3);
	cv::imshow("Lomo", result);
}

void cartoon(cv::Mat img) { // cartoon effect
	cv::Mat imgMedian; // apply median filter to remove possible noise
	cv::medianBlur(img, imgMedian, 7);
	// Canny function used to detect edges
	cv::Mat imgCanny;
	cv::Canny(imgMedian, imgCanny, 50, 150);
	// edge swelling
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
	cv::dilate(imgCanny, imgCanny, kernel);
	// scale the edge value to 1 and flip the value
	imgCanny = imgCanny / 255;
	imgCanny = 1 - imgCanny;
	// convert to float type
	cv::Mat imgCannyf;
	imgCanny.convertTo(imgCannyf, CV_32FC3);
	cv::blur(imgCannyf, imgCannyf, cv::Size(5, 5)); // blur edge
	// apply bilateral filter for color uniformity
	cv::Mat imgBF;
	cv::bilateralFilter(img, imgBF, 9, 150.0, 150.0);
	cv::Mat result = imgBF / 25; // truncate color
	result = result * 25;
	// create 3 channels for edge
	cv::Mat imgCanny3C;
	cv::Mat cannyChannels[] = {imgCannyf, imgCannyf, imgCannyf};
	cv::merge(cannyChannels, 3, imgCanny3C);

	cv::Mat resultf; // convert 'result' to float
	result.convertTo(resultf, CV_32FC3);
	cv::multiply(resultf, imgCanny3C, resultf); // multiply color and edge matrix
	resultf.convertTo(result, CV_8UC3); // convert 'result' to 8-bit image
	cv::imshow("Cartoon", result);
}

void understandMat(cv::Mat img) {
	cv::Mat img1(240, 320, CV_8U, 100);
	cv::imshow("Image", img1);
	cv::waitKey(0);

	cv::Mat img2(240, 320, CV_8UC3, cv::Scalar(0, 0, 255));
	cv::imshow("Image", img2);
	cv::waitKey(0);

	cv::Mat img3 = img;

	cv::Mat img4(img3);
	img1 = img3;

	img3.copyTo(img2);
	cv::Mat img5 = img3.clone();

	cv::flip(img3, img3, 1); // 0: H, 1: V

	cv::imshow("Image 3", img3);
	cv::imshow("Image 1", img1);
	cv::imshow("Imgae 2", img2);
	cv::imshow("Image 4", img4);
	cv::imshow("Image 5", img5);
	cv::waitKey(0);

	cv::Mat gray(500, 500, CV_8U, 50);

	cv::imshow("Image", gray);
	cv::waitKey(0);

	img1 = img;
	img1.convertTo(img2, CV_32F, 1 / 255, 0);
	cv::imshow("Image", img2);
	cv::waitKey(0);
}

void imageROI(cv::Mat img) {
	cv::putText(img, "IMG",
				cv::Point(100, 400),
				cv::FONT_HERSHEY_PLAIN,
				10, 255, 5);
	// img(cv::Range(480, 20), cv::Range(480, 20));
	// img.colRange(start, end)/img.rowRange(start, end)
	cv::Mat imageROI(img, // Region of Interest
					 cv::Rect(400, 400, 100, 100));
	cv::Mat logo(100, 100, CV_8UC3, cv::Scalar(0, 255, 0)); // BGR format
	cv::Mat mask(logo);
	logo.copyTo(imageROI, mask);
	cv::imshow("Original Image", img);
	std::cout << img.channels() << std::endl;
}