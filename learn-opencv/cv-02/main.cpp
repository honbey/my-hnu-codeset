/******************************************************************************************
 * Project Name: two
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-01-14
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-01-14s
 *
 * Description: the 2nd learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

static cv::Scalar randomColor(cv::RNG&); // choose a random color

cv::Mat removeLight(cv::Mat, cv::Mat, int); // remove the light from image
cv::Mat calculateLightPattern(cv::Mat); // calculate to create a light pattern/background
cv::Mat imageThreshold(cv::Mat, int lightFlag); // binarize image
void connectedComponents(cv::Mat); // detect target from image
void connectedComponentsStats(cv::Mat); // detect and show info from image
void findContoursBasic(cv::Mat); // find contours from image

int main(int argc, char const *argv[]) {
	if (argc != 3) { // parameter error
		std::cout << "Usage: ./Testing <Image Path> <Image Path>" << std::endl;
		return -1;
	}

	cv::namedWindow("Two"); // create a window
	cv::Mat img = cv::imread(argv[1]); // read a image
	if (!img.data) { // image is empty or can't open it
		std::cout << "error: No image data" << std::endl;
		return -2;
	}

	cv::Mat imgTemp = cv::imread(argv[2]);
	imgTemp = removeLight(img, imgTemp, 0);
	//imgTemp = removeLight(img, calculateLightPattern(img), 0);
	cv::medianBlur(imgTemp, imgTemp, 9);
	cv::imshow("Two", imgTemp);
	imgTemp = imageThreshold(imgTemp, 1);
	findContoursBasic(imgTemp);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

cv::Mat removeLight(cv::Mat withLight, cv::Mat lightImg, int flag) {
	cv::Mat aux;
	if (flag == 0) aux = lightImg - withLight; // normalization: division or different
	else {
		cv::Mat withLight32, lightImg32; // require change image to 32 float for division
		withLight.convertTo(withLight32, CV_32F);
		lightImg.convertTo(lightImg32, CV_32F);
		aux = 1 - (withLight32 / lightImg32); // divide the image by lightImg
		aux.convertTo(aux, CV_8U, 255); // convert 8-bit format and scale
	}
	return aux;
}

cv::Mat calculateLightPattern(cv::Mat img) {
	cv::Mat lightImg;
	// basic and effective way to calculate the light pattern from img
	cv::blur(img, lightImg, cv::Size(img.cols / 3, img.rows / 3));
	return lightImg;
}

cv::Mat imageThreshold(cv::Mat img, int lightFlag) { // I(x, y), (x, y) < threshold ? 0 : 1(255)
	cv::Mat imgThr; // binarize image for segment
	if (lightFlag == 0) { // if removed light
		cv::threshold(img, imgThr, 140, 255, cv::THRESH_BINARY_INV);
	} else {
		cv::threshold(img, imgThr, 30, 255, cv::THRESH_BINARY);
	}
	return imgThr;
}

static cv::Scalar randomColor(cv::RNG &rng) {
	auto icolor = (unsigned)rng;
	return cv::Scalar(icolor & 255, (icolor>>8) & 255, (icolor>>16) & 255);
}

void connectedComponents(cv::Mat img) {
	// use connected components to divied our image im multiple connected conponent objects
	cv::Mat labels;
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

	auto num_objects = cv::connectedComponents(img, labels);
	if (num_objects < 2) {
		std::cout << "no objects detected" << std::endl;
		return;
	} else std::cout << "Number of objects detected: " << num_objects - 1 << std::endl;
	cv::Mat resule = cv::Mat::zeros(img.rows, img.cols, CV_8UC3); // create resule image coloring the objects
	cv::RNG rng(0xFFFFFFFF);
	for (auto i = 1; i < num_objects; i++) {
		cv::Mat mask = labels == i;
		resule.setTo(randomColor(rng), mask);
	}
	cv::imshow("Result", resule);
}

void connectedComponentsStats(cv::Mat img) { // with Stats
	cv::Mat labels, stats, centroids;
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

	auto num_objects = cv::connectedComponentsWithStats(img, labels, stats, centroids);
	if (num_objects < 2) {
		std::cout << "no objects detected" << std::endl;
		return;
	} else std::cout << "Number of objects detected: " << num_objects - 1 << std::endl;
	// create resule image coloring the objects and show area
	cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
	cv::RNG rng(0xFFFFFFFF);
	for (auto i = 1; i < num_objects; i++) {
		std::cout << "Object " << i 
			<< " with pos: " << centroids.at<cv::Point2d>(i)
			<< " wiht area " << stats.at<int>(i, cv::CC_STAT_AREA) << std::endl;
		cv::Mat mask = labels == i;
		result.setTo(randomColor(rng), mask);
		std::stringstream ss; // draw text with area
		ss << "area: " << stats.at<int>(i, cv::CC_STAT_AREA);
		cv::putText(result, ss.str(),
			centroids.at<cv::Point2d>(i),
			cv::FONT_HERSHEY_SIMPLEX,
			1, cv::Scalar(255, 255, 255));
	}
	cv::imshow("Result", result);
}

void findContoursBasic(cv::Mat img) {
	std::vector<std::vector<cv::Point>> contours;
	cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

	cv::findContours(img, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	cv::Mat result = cv::Mat::zeros(img.rows, img.cols, CV_8UC3);
	if (contours.size() == 0) { // check the number of objects detected
		std::cout << "no objects detected" << std::endl;
		return;
	} else std::cout << "Number of objects detected: " << contours.size() << std::endl;
	cv::RNG rng(0xFFFFFFFF);
	for (auto i = 0; i < contours.size(); i++) {
		cv::drawContours(result, contours, i, randomColor(rng));
		imshow("Result", result);
	}
}