// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneCameras.h"
#include "ObstaclesDetection.h"
#include "CameraHelper.h"
#include <vector>

int main() 
{
	const std::string kGuppyWindowTitle = "GUPPY camera view";
	const std::string kGuppyThresholdWindowTitle = "GUPPY camera view with Threshold";
	const std::string kGuppySobelWindowTitle = "GUPPY camera view - Sobel";
	const std::string kGuppyHoughWindowTitle = "GUPPY camera view - Hough";
	const std::string kPikeOneWindowTitle = "PIKE 1 camera view";
	const std::string kPikeTwoWindowTitle = "PIKE 2 camera view";

	CraneCameras cameras;
	FramePtr guppy_frame, pike1_frame, pike2_frame; 
	cv::Mat guppy_mat, pike1_mat, pike2_mat, guppy_thresh, guppy_sobel, guppy_erode, guppy_morph;
//	cv::Mat element = getStructuringElement(cv::MORPH_CLOSE, cv::Size(2 * 1 + 1, 2 * 1 + 1), cv::Point(3, 3));
	int threshold_value = 0;
	const int threshold_max = 255;

	cv::namedWindow(kGuppyWindowTitle, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kGuppyThresholdWindowTitle, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kGuppySobelWindowTitle, cv::WINDOW_AUTOSIZE);

	//cv::namedWindow(PIKE_1_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);
	//cv::namedWindow(PIKE_2_WINDOW_TITLE, cv::WINDOW_AUTOSIZE);

	cv::createTrackbar("Threshold", kGuppyThresholdWindowTitle, &threshold_value, threshold_max, NULL);
	while (1) {
		guppy_frame = cameras.GetFrame(0);
		//pike1_frame = cameras.GetFrame(1);
		//pike2_frame = cameras.GetFrame(2);

		guppy_mat = CameraHelper::FrameToCVMat(guppy_frame, CV_8UC1);
		//pike1_mat = CameraHelper::FrameToCVMat(pike1_frame, CV_8UC1);
		//pike2_mat = CameraHelper::FrameToCVMat(pike2_frame, CV_8UC1);

		cv::cvtColor(guppy_mat, guppy_mat, CV_BayerBG2RGB);
		cv::threshold(guppy_mat, guppy_thresh, threshold_value, 255, cv::THRESH_BINARY);
		cv::Sobel(guppy_thresh, guppy_sobel, CV_8U, 1, 1);
		//cv::erode(guppy_sobel, guppy_sobel, 
		//cv::morphologyEx(guppy_thresh, guppy_sobel, cv::MORPH_CLOSE, kernel_close);
		cv::Canny(guppy_thresh, guppy_sobel, threshold_value, 255);
		

		cv::imshow(kGuppyWindowTitle, guppy_mat);
		cv::imshow(kGuppyThresholdWindowTitle, guppy_thresh);
		cv::imshow(kGuppySobelWindowTitle, guppy_sobel);

		//cv::imshow(PIKE_1_WINDOW_TITLE, pike1_mat);
		//cv::imshow(PIKE_2_WINDOW_TITLE, pike2_mat); 

		if (cv::waitKey(15) >= 0) 
			break;
	}

    return 0;
}

