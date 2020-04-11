/******************************************************************************************
 * Project Name: two
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-01-14
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-01-14
 *
 * Description: the 2nd learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

static cv::Scalar randomColor(cv::RNG&); // choose a random color
/* Part 1 */
cv::Mat removeLight(cv::Mat, cv::Mat, int); // remove the light from image
cv::Mat calculateLightPattern(cv::Mat); // calculate to create a light pattern/background
cv::Mat imageThreshold(cv::Mat, int); // binarize image
void connectedComponents(cv::Mat); // detect target from image
void connectedComponentsStats(cv::Mat); // detect and show info from image
void findContoursBasic(cv::Mat); // find contours from image

/******************************************************************************************
 * The steps of Machine Learning:
 * 1. Protreatment ............. (Part 1)
 * 2. Segment .................. (Part 1)
 * 3. Feature extraction ....... (Part 2)
 * 4. Classification results ... (Part 2)
 * 5. Aftertreatment ........... (Part 2)
 ******************************************************************************************/

/* Part 2 */
#include "MultipleImageWindow.h"
#include <memory>
std::shared_ptr<MultipleImageWindow> miw;

bool readFolderAndExtractFeatures
(std::string folder, int label, int num_for_test, std::vector<float> &trainingData, 
	std::vector<int> &responsesData, std::vector<float> &testData, std::vector<float> &testResponsesData);
void plotTrainData(cv::Mat trainData, cv::Mat labels, float *error=NULL);
std::vector<std::vector<float>> 
extractFeatures(cv::Mat img, std::vector<int> *left=NULL, std::vector<int> *top=NULL);
cv::Ptr<cv::ml::SVM> svm;
void trainAndTest();

int main(int argc, char const *argv[]) {
	if (argc != 2) { // parameter error
		std::cout << "Usage: ./Testing <Image Path>" << std::endl;
		return -1;
	}

	//cv::namedWindow("Two"); // create a window
	cv::Mat img = cv::imread(argv[1]); // read a image
	if (!img.data) { // image is empty or can't open it
		std::cout << "error: No image data" << std::endl;
		return -2;
	}
	
	miw = std::make_shared<MultipleImageWindow>("Main window", 2, 2, cv::WINDOW_AUTOSIZE);

	trainAndTest();
	cv::Mat input = img.clone();
	cv::Mat bgImg = cv::imread("../../../data/pattern.pgm");
	cv::medianBlur(input, input, 3);
	cv::medianBlur(bgImg, bgImg, 3);
	input = removeLight(input, bgImg, 1); // apply the light pattern
	input = imageThreshold(input, 1);
	cv::cvtColor(input, input, COLOR_RGB2GRAY);

	std::vector<int> pos_top, pos_left;
	std::vector<std::vector<float>> features= extractFeatures(input, &pos_left, &pos_top);
	std::cout << "Num objects extracted features " << features.size() << std::endl;

	for (auto i = 0; i < features.size(); i++){
		std::cout << "Data Area AR: " << features[i][0] << " " << features[i][1] << std::endl;

		cv::Mat trainingDataMat(1, 2, CV_32FC1, &features[i][0]);
		std::cout << "Features to predict: " << trainingDataMat << std::endl;
		float result = svm->predict(trainingDataMat);
		std::cout << result << std::endl;

		std::stringstream ss;
		cv::Scalar color;
		if (result == 0){
			color = cv::Scalar(0, 255, 0); // NUT, color is Green
			ss << "NUT";
		} else if (result == 1){
			color = cv::Scalar(255, 0, 0); // RING, color is blue
			ss << "RING";
		} else if (result == 2){
			color = cv::Scalar(0, 0, 255); // SCREW, color is red
			ss << "SCREW";
		}
		cv::putText(img, ss.str(), 
			cv::Point2d(pos_left[i], pos_top[i]), 
			cv::FONT_HERSHEY_SIMPLEX, 0.5, color);
	}

	//std::vector<int> results = evaluate(features);
	miw->addImage("Binary image", bgImg); // show images
	miw->addImage("Result", img);
	miw->render();

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
	cv::Mat lightImg; // basic and effective way to calculate the light pattern from img
	cv::blur(img, lightImg, cv::Size(img.cols / 3, img.rows / 3));
	return lightImg;
}

cv::Mat imageThreshold(cv::Mat img, int lightFlag) { // I(x, y), (x, y) < threshold ? 0 : 1(255)
	cv::Mat imgThr; // binarize image for segment
	if (lightFlag == 0) cv::threshold(img, imgThr, 140, 255, cv::THRESH_BINARY_INV); // if removed light
	else cv::threshold(img, imgThr, 30, 255, cv::THRESH_BINARY);
	return imgThr;
}

static cv::Scalar randomColor(cv::RNG &rng) {
	auto icolor = (unsigned)rng;
	return cv::Scalar(icolor & 255, (icolor>>8) & 255, (icolor>>16) & 255);
}

void connectedComponents(cv::Mat img) {
	cv::Mat labels; // use connected components to divied our image im multiple connected conponent objects
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
			0.5, cv::Scalar(255, 255, 255));
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

void plotTrainData(cv::Mat trainData, cv::Mat labels, float *error) {
	float area_max, ar_max, area_min, ar_min;
	area_max = ar_max = 0;
	area_min = ar_min = 99999999;
	for(auto i=0; i< trainData.rows; i++) { // get the min and max of each feature for normalize plot image
		float area= trainData.at<float>(i, 0);
		float ar= trainData.at<float>(i, 1);
		if(area > area_max) area_max = area;
		if( ar > ar_max ) ar_max = ar;
		if(area < area_min) area_min = area;
		if(ar < ar_min) ar_min = ar;
	}

	cv::Mat plot= cv::Mat::zeros(512, 512, CV_8UC3); // create Image for plot
	// plot each of two features in a 2D graph using an image where x is area and y is aspect ratio
	for(auto i = 0; i< trainData.rows; i++){ // set the x y pos for each data
		float area = trainData.at<float>(i, 0);
		float ar = trainData.at<float>(i, 1);
		int x = static_cast<int>(512.0f * ((area - area_min) / (area_max - area_min)));
		int y = static_cast<int>(512.0f * ((ar - ar_min) / (ar_max - ar_min)));

		int label= labels.at<int>(i); // get lable
		cv::Scalar color; // set color depend of label
		if(label == 0) color = cv::Scalar(0, 255, 0); // NUT
		else if(label == 1) color = cv::Scalar(255, 0, 0); // RING
		else if(label == 2) color = cv::Scalar(0, 0, 255); // SCREW

		cv::circle(plot, cv::Point(x, y), 3, color, -1, 8);
	}

	if(error != NULL){
	std::stringstream ss;
	ss << "Error: " << *error << "\%"; 
		cv::putText(plot, ss.str().c_str(), 
			cv::Point(20, 512 - 40), 
			cv::FONT_HERSHEY_SIMPLEX, 
			0.75, cv::Scalar(200, 200, 200), 1, 
			cv::LINE_AA);
	}
	miw->addImage("Plot", plot);
}

std::vector<std::vector<float>> 
extractFeatures(cv::Mat img, std::vector<int> *left, std::vector<int> *top) {
	std::vector<std::vector<float>> result;
	std::vector<std::vector<cv::Point>> contours;
	cv::Mat input = img.clone();

	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(input, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
	if (contours.size() == 0) return result; // check the number of objects detected
	cv::RNG rng(0xFFFFFFFF);
	for (auto i = 0; i < contours.size(); i++) {
		cv::Mat mask = cv::Mat::zeros(img.rows, img.cols, CV_8UC1);
		cv::drawContours(mask, contours, i, cv::Scalar(1), cv::FILLED, cv::LINE_8, hierarchy, 1);
		cv::Scalar area_s = cv::sum(mask);
		float area = area_s[0];
		if (area > 500.0) { // if the area is greter than min
			cv::RotatedRect r = cv::minAreaRect(contours[i]);
			float width = r.size.width;
			float height = r.size.height;
			float ar = (width < height) ? height / width : width / height; // h/w or w/h rate
			std::vector<float> row;
			row.push_back(area);
			row.push_back(ar);
			result.push_back(row);
			if (left != NULL) left->push_back(static_cast<int>(r.center.x));
			if (top != NULL) top->push_back(static_cast<int>(r.center.y));
			// add image to the multiple image window class, see the class on full github code
			miw->addImage("Extract Features", mask * 255);
			miw->render();
			cv::waitKey(10);
		}
	}
	return result;
}

bool readFolderAndExtractFeatures(std::string folder, int label, int num_for_test,
	std::vector<float> &trainingData, std::vector<int> &responsesData,
	std::vector<float> &testData, std::vector<float> &testResponsesData) {
	cv::VideoCapture cap;
	if (!cap.open(folder)) {
		std::cout << "Can't open the folder images" << std::endl;
		return false;
	}

	cv::Mat frame;
	int img_index = 0;
	for (; cap.read(frame); img_index++) {
		cv::Mat bgImg = cv::imread("../../../data/pattern.pgm");
		cv::medianBlur(frame, frame, 3);	
		cv::medianBlur(bgImg, bgImg, 3);
		frame = removeLight(frame, bgImg, 1); // apply the light pattern
		frame = imageThreshold(frame, 1);
		cv::cvtColor(frame, frame, COLOR_RGB2GRAY);
		std::vector<std::vector<float>> features = extractFeatures(frame); // extract features
		for (auto i = 0; i < features.size(); i++) {
			if (img_index >= num_for_test) {
				trainingData.push_back(features[i][0]);
				trainingData.push_back(features[i][1]);
				responsesData.push_back(label);
			} else {
				testData.push_back(features[i][0]);
				testData.push_back(features[i][1]);
				testResponsesData.push_back(static_cast<float>(label));
			}
		}
	}
	return true;
}

void trainAndTest() {
	std::vector<float> trainingData;
	std::vector<int> responsesData;
	std::vector<float> testData;
	std::vector<float> testResponsesData;

	int num_for_test = 20;
	readFolderAndExtractFeatures("../../../data/nut/tuerca_%04d.pgm",
		0, num_for_test,
		trainingData, responsesData,
		testData, testResponsesData); // get the nut images
	readFolderAndExtractFeatures("../../../data/ring/arandela_%04d.pgm",
		1, num_for_test,
		trainingData, responsesData,
		testData, testResponsesData); // get and process the ring images
	readFolderAndExtractFeatures("../../../data/screw/tornillo_%04d.pgm",
		2, num_for_test,
		trainingData, responsesData,
		testData, testResponsesData); // get and process the screw images
	std::cout << "Num of train samples: " << responsesData.size() << std::endl;
	std::cout << "Num of test samples: " << testResponsesData.size() << std::endl;
	// merge all data
	cv::Mat trainingDataMat(trainingData.size() / 2, 2, CV_32FC1, &trainingData[0]);
	cv::Mat responsesDataMat(responsesData.size(), 1, CV_32SC1, &responsesData[0]);
	cv::Mat testDataMat(testData.size() / 2, 2, CV_32FC1, &testData[0]);
	cv::Mat testResponsesDataMat(testResponsesData.size(), 1, CV_32FC1, &testResponsesData[0]);

	cv::Ptr<cv::ml::TrainData> tdata = cv::ml::TrainData::create(trainingDataMat, cv::ml::ROW_SAMPLE, responsesDataMat);

	svm = cv::ml::SVM::create();
	svm->setType(cv::ml::SVM::C_SVC);
	svm->setNu(0.05);
	svm->setKernel(cv::ml::SVM::CHI2);
	svm->setDegree(1.0);
	svm->setGamma(2.0);
	svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
	svm->train(tdata);

	if (testResponsesData.size() > 0) {
		std::cout << "Evaluation\n==========" << std::endl;
		cv::Mat testPredict; // test the ML model
		svm->predict(testDataMat, testPredict);
		std::cout << "Prediction Done" << std::endl;
		cv::Mat errorMat = testPredict != testResponsesDataMat;
		float error = 100.0f * cv::countNonZero(errorMat) / testResponsesData.size(); // error calculation
		std::cout << "Error: " << error << "\%" << std::endl;
		plotTrainData(trainingDataMat, responsesDataMat, &error); // plot training data with error label
	} else plotTrainData(trainingDataMat, responsesDataMat);
}