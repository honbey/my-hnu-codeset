/******************************************************************************************
 * Project Name: three
 *
 * Honbey, honbey@honbey.com
 * Created On 2020-02-03
 * Copyright (c) 2020. All rights reserved.
 *
 * Date: 2019-02-03
 *
 * Description: the 3rd learning record of OpenCV.
 *   C++ program for learning OpenCV.
 *
 ******************************************************************************************/

#include <iostream>
#include <opencv2/opencv.hpp>

/**/

void earDetector();
void overlayFacemask(cv::Mat);
void overlayMoustache(cv::Mat);
void overlayNose(cv::Mat);
void overlaySunglasses(cv::Mat);

int main(int argc, char const *argv[]) {
	if (argc != 2) { // parameter error
		std::cout << "Usage: ./Testing <Image Path>" << std::endl;
		return -1;
	}

	//cv::namedWindow("Three"); // create a window
	cv::Mat img = cv::imread(argv[1]); // read a image
	if (!img.data) { // image is empty or can't open it
		std::cout << "error: No image data" << std::endl;
		return -2;
	}
	
	//overlayFacemask(img);
	overlaySunglasses(img);

	cv::waitKey(0);
	cv::destroyAllWindows();
	return 0;
}

void earDetector() {
	cv::VideoCapture cap;
	cap.open("mstp://"); // create the capture object and receive video
	if(!cap.isOpened()) return;
	cv::Mat frame, frameGray; // current frame
	
	float scalingFactor = 0.75; // scaling factor to resize the input frames from the webcam
	std::string lEarCascadeName = "../../../data/resources/haarcascade_mcs_leftear.xml";
	std::string rEarCascadeName = "../../../data/resources/haarcascade_mcs_rightear.xml";
	cv::CascadeClassifier lEarCascade, rEarCascade;
	lEarCascade.load(lEarCascadeName);
	rEarCascade.load(rEarCascadeName);

	std::vector<cv::Rect> lEar, rEar;

	while(true) {
		cap >> frame; // capture the current frame
		cv::resize(frame, frame, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
		cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
		cv::equalizeHist(frameGray, frameGray); // equalize the histogram
		lEarCascade.detectMultiScale(frameGray, lEar, 1.1, 2, 0 | 2, cv::Size(30, 30)); // detect ear
		rEarCascade.detectMultiScale(frameGray, rEar, 1.1, 2, 0 | 2, cv::Size(30, 30));

		for(auto& lE : lEar) {
			cv::Rect lEarRect(lE.x, lE.y, lE.width, lE.height);
			cv::rectangle(frame, lEarRect, cv::Scalar(0, 255, 0), 4); // draw green rectangle aroud left ear
		}
		for(auto& rE : rEar) {
			cv::Rect rEarRect(rE.x, rE.y, rE.width, rE.height);
			cv::rectangle(frame, rEarRect, cv::Scalar(0, 255, 0), 4); // draw green rectangle aroud left ear
		}

		cv::imshow("Ear Detect", frame);

		char c = cv::waitKey(30);
		if(c == 27) break;
	}

	cap.release();
}

void overlayFacemask(cv::Mat img) {
	std::string faceCascadeName = "../../../data/resources/haarcascade_frontalface_alt.xml";
	cv::CascadeClassifier faceCascade;
	faceCascade.load(faceCascadeName);

	cv::Mat faceMask = cv::imread("../../../data/resources/mask.jpg");
	if(!faceMask.data) return; // if can't open the mask, stop the execution

	cv::Mat imgGray, imgROI, maskedImg, maskedFace;
	cv::Mat faceMaskSmall, grayMaskSmall, grayMaskSmallThresh, grayMaskSmallThreshInv;

	float scalingFactor = 0.75; // scaling factor to resize the input frames from the image
	std::vector<cv::Rect> faces;

	cv::resize(img, img, cv::Size(), scalingFactor, scalingFactor, cv::INTER_AREA);
	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(imgGray, imgGray);
	faceCascade.detectMultiScale(imgGray, faces, 1.1, 2, 0 | 2, cv::Size(30, 30));
	for(auto& face : faces) {
		cv::Rect faceRect(face.x, face.y, face.width, face.height);
		// custom parameters to make the mask fit your face
		int x = face.x - int(0.1 * face.width);
		int y = face.y - int(0.0 * face.height);
		int w = int(1.1 * face.width);
		int h = int(1.3 * face.height);
		// extract region of interest covering your face
		imgROI = img(cv::Rect(x, y, w, h));
		cv::resize(faceMask, faceMaskSmall, cv::Size(w, h));
		cv::cvtColor(faceMaskSmall, grayMaskSmall, cv::COLOR_BGR2GRAY);
		// threshold the above image to isolate the pixels associated only with the face mask
		cv::threshold(grayMaskSmall, grayMaskSmallThresh, 230, 255, cv::THRESH_BINARY_INV);
		// create mask by inverting the above image
		cv::bitwise_not(grayMaskSmallThresh, grayMaskSmallThreshInv);
		// use bitwise "AND" operator to extract precise boundary of face mask
		cv::bitwise_and(faceMaskSmall, faceMaskSmall, maskedFace, grayMaskSmallThresh);
		// use bitwise "AND" operator to overlay face mask
		cv::bitwise_and(imgROI, imgROI, maskedImg, grayMaskSmallThreshInv);
		// add the above masked images and place it in the original frame ROI to create the final image
		if(x > 0 && y > 0 && x + w < img.cols && y + h < img.rows) cv::add(maskedFace, maskedImg, img(cv::Rect(x, y, w, h)));
	}
	cv::imshow("Mask", img);
	cv::waitKey(0);
}

void overlaySunglasses(cv::Mat img) {
	std::string faceCascadeName = "../../../data/resources/haarcascade_frontalface_alt.xml";
	cv::CascadeClassifier faceCascade;
	faceCascade.load(faceCascadeName);
	std::string eyeCascadeName = "../../../data/resources/haarcascade_eye.xml";
	cv::CascadeClassifier eyeCascade;
	eyeCascade.load(eyeCascadeName);

	cv::Mat eyeMask = cv::imread("../../../data/resources/glasses.jpg");
	if(!eyeMask.data) return;

	cv::Mat imgGray, imgROI, maskedImg, maskedEye;
	cv::Mat eyeMaskSmall, grayMaskSmall, grayMaskSmallThresh, grayMaskSmallThreshInv;

	std::vector<cv::Rect> faces;

	cv::resize(img, img, cv::Size(), 0.75, 0.75, cv::INTER_AREA);
	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(imgGray, imgGray);
	faceCascade.detectMultiScale(imgGray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	std::vector<cv::Point> centers;
	for(auto i = 0; i < faces.size(); i++) { // draw green circles around the eyes
		cv::Mat faceROI = imgGray(faces[i]);
		std::vector<cv::Rect> eyes; // in each face, detect eyes
		eyeCascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
		for(auto j = 0; j < eyes.size(); j++) {
			cv::Point center(faces[i].x + eyes[j].x + int(eyes[j].width * 0.5),
				faces[i].y + eyes[j].y + int(eyes[j].height * 0.5));
			centers.push_back(center);
		}
	}
	if(centers.size() == 2) { // overlay sunglasses only if both eyes are detected
		cv::Point lPoint, rPoint;
		if(centers[0].x < centers[1].x) { // identify the left and right eyes
			lPoint = centers[0];
			rPoint = centers[1];
		} else {
			lPoint = centers[1];
			rPoint = centers[0];
		}
		int w = int(2.3 * (rPoint.x - lPoint.x));
		int h = int(0.4 * w);
		int x = lPoint.x - int(0.25 * w);
		int y = rPoint.y - int(0.50 * h);

		imgROI = img(cv::Rect(x, y, w, h));
		cv::resize(eyeMask, eyeMaskSmall, cv::Size(w, h));
		cv::cvtColor(eyeMaskSmall, grayMaskSmall, cv::COLOR_BGR2GRAY);
		cv::threshold(grayMaskSmall, grayMaskSmallThresh, 245, 255, cv::THRESH_BINARY_INV);
		cv::bitwise_not(grayMaskSmallThresh, grayMaskSmallThreshInv);
		cv::bitwise_and(eyeMaskSmall, eyeMaskSmall, maskedEye, grayMaskSmallThresh);
		cv::bitwise_and(imgROI, imgROI, maskedImg, grayMaskSmallThreshInv);
		cv::add(maskedEye, maskedImg, img(cv::Rect(x, y, w, h)));
	}
	cv::imshow("Glasses", img);
	cv::waitKey(0);
}