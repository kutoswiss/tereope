// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneCameras.h"
#include "ObstaclesDetection.h"
#include "CameraHelper.h"
#include <vector>
#include <iostream>
#include <thread>

void GuppyCameraThread(CraneCameras &cameras, const std::string kWindowTitle, ObstaclesDetection &obstacle_detection) {
	FramePtr frame;
	cv::Mat frame_mat;

	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	while (1) {
		frame = cameras.GetFrame(0);
		frame_mat = CameraHelper::FrameToCVMat(frame, CV_8UC1);
		cv::cvtColor(frame_mat, frame_mat, CV_BayerBG2RGB);

		obstacle_detection.SetRawFrame(frame_mat);
		obstacle_detection.Detect();

		cv::imshow(kWindowTitle, obstacle_detection.GetFrameWithRectangles(true));
		if (cv::waitKey(15) >= 0) 
			break;
	}

	cv::destroyWindow(kWindowTitle);
}

int main() {
	const std::string kGuppyWindowTitle = "GUPPY camera view";
	const std::string kGuppyThresholdWindowTitle = "GUPPY camera view with Threshold";
	const std::string kGuppySobelWindowTitle = "GUPPY camera view - Sobel";
	const std::string kGuppyHoughWindowTitle = "GUPPY camera view - Hough";
	const std::string kPikeOneWindowTitle = "PIKE 1 camera view";
	const std::string kPikeTwoWindowTitle = "PIKE 2 camera view";

	CraneCameras cameras;
	FramePtr guppy_frame, pike1_frame, pike2_frame; 
	cv::Mat guppy_mat, pike1_mat, pike2_mat, guppy_thresh, guppy_sobel, guppy_erode, guppy_contours;
	ObstaclesDetection obstacle_detection;
	std::vector<std::vector<cv::Point>> contours;
	int threshold_value = 25;
	const int threshold_max = 255;


	std::thread guppy_cam_thread(
		GuppyCameraThread, 
		std::ref(cameras), 
		kGuppyWindowTitle, 
		std::ref(obstacle_detection));
	guppy_cam_thread.join();


	



	//cv::namedWindow(PIKE_1_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);
	//cv::namedWindow(PIKE_2_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);

	//cv::createTrackbar("Threshold", kGuppyThresholdWindowTitle, &threshold_value, threshold_max, NULL);
	//while (1) {
	//	guppy_frame = cameras.GetFrame(0);
	//	//pike1_frame = cameras.GetFrame(1);
	//	//pike2_frame = cameras.GetFrame(2);

	//	guppy_mat = CameraHelper::FrameToCVMat(guppy_frame, CV_8UC1);
	//	//pike1_mat = CameraHelper::FrameToCVMat(pike1_frame, CV_8UC1);
	//	//pike2_mat = CameraHelper::FrameToCVMat(pike2_frame, CV_8UC1);

	//	cv::cvtColor(guppy_mat, guppy_mat, CV_BayerBG2RGB);
	//	cv::threshold(guppy_mat, guppy_thresh, threshold_value, 255, cv::THRESH_BINARY);
	//	cv::Sobel(guppy_thresh, guppy_sobel, CV_8U, 1, 1);
	//	//cv::erode(guppy_sobel, guppy_sobel, 
	//	//cv::morphologyEx(guppy_thresh, guppy_sobel, cv::MORPH_CLOSE, kernel_close);
	//	cv::Canny(guppy_thresh, guppy_sobel, threshold_value, 255);
	//	

	//	cv::imshow(kGuppyWindowTitle, guppy_mat);
	//	cv::imshow(kGuppyThresholdWindowTitle, guppy_thresh);
	//	cv::imshow(kGuppySobelWindowTitle, guppy_sobel);


	//	//cv::imshow(PIKE_1_WINDOW_TITLE, pike1_mat);
	//	//cv::imshow(PIKE_2_WINDOW_TITLE, pike2_mat); 

	//	if (cv::waitKey(15) >= 0) 
	//		break;
	//}


	//cv::destroyAllWindows();
	//cv::findContours(guppy_sobel, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	//// Get obstacles contours
	//std::vector<std::vector<cv::Point>> obstaclesContours;
	//for (std::vector<std::vector<cv::Point>>::iterator it = contours.begin(); 
	//	it != contours.end(); it++) {
	//	if (cv::contourArea(*it) > 125)
	//		obstaclesContours.push_back(*it);
	//}

	//// Calculate obstacles rectangles
	//std::vector<cv::Rect2d> obstaclesRects;
	//for (std::vector<std::vector<cv::Point>>::iterator it = obstaclesContours.begin();
	//	it != obstaclesContours.end(); it++) {
	//	obstaclesRects.push_back(cv::boundingRect(*it));
	//}

	//// Draw obstacles rectangles
	//for (std::vector<cv::Rect2d>::iterator it = obstaclesRects.begin();
	//	it != obstaclesRects.end(); it++) {
	//	cv::rectangle(guppy_mat, *it, cv::Scalar(0, 255, 0), 3);
	//}

	//// Calculate obstacles rotated rectangles
	//std::vector<cv::RotatedRect> obstaclesRotatedRects;
	//for (std::vector<std::vector<cv::Point>>::iterator it = obstaclesContours.begin();
	//	it != obstaclesContours.end(); it++) {
	//	obstaclesRotatedRects.push_back(cv::minAreaRect(*it));
	//}

	//// Draw rotated rectangles (non filled rectangles)
	//std::vector<std::vector<cv::Point>> points;
	//for (std::vector<cv::RotatedRect>::iterator it = obstaclesRotatedRects.begin();
	//	it != obstaclesRotatedRects.end(); it++) {
	//	cv::Point2f vertices[4];
	//	std::vector<cv::Point> pts;

	//	(*it).points(vertices);
	//	for (int i = 0; i < 4; i++) {
	//		pts.push_back(vertices[i]);
	//	}

	//	points.push_back(pts);
	//}
	//cv::polylines(guppy_mat, points, true, cv::Scalar(0, 255, 0), 3, CV_AA);

	//// Print informations of rectangles
	//for (std::vector<cv::RotatedRect>::iterator it = obstaclesRotatedRects.begin();
	//	it != obstaclesRotatedRects.end(); it++) {
	//	std::stringstream ss;
	//	ss << "Origin: " << (int)((*it).center.x) << "," << (int)((*it).center.y) << "; Angle: " << std::setprecision(2) << (*it).angle << " deg.";
	//	
	//	cv::Point origin((*it).center.x - 10 - (*it).size.height/2, (*it).center.y - 10 - (*it).size.width/2);
	//	
	//	cv::putText(guppy_mat, ss.str(), origin, cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(0, 255, 0));
	//}

	//// Draw obstacles rotated rectangles
	//for (std::vector<cv::RotatedRect>::iterator it = obstaclesRotatedRects.begin();
	//	it != obstaclesRotatedRects.end(); it++) {
	//	cv::Point2f vertices2f[4];
	//	cv::Point vertices[4];

	//	(*it).points(vertices2f);
	//	for (int i = 0; i < 4; i++)
	//		vertices[i] = vertices2f[i];

	//	cv::fillConvexPoly(guppy_mat, vertices, 4, cv::Scalar(0, 255, 0), CV_AA);
	//}

	//cv::drawContours(guppy_sobel, contours, -1, cv::Scalar::all(255));
	//cv::imshow(kGuppySobelWindowTitle, guppy_mat);
	//cv::waitKey(0);


    return 0;
}

