// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneCameras.h"
#include "ObstaclesDetection.h"
#include "CameraHelper.h"
#include <vector>
#include <iostream>
#include <thread>

void GuppyCameraThread(CraneCameras &cameras, ObstaclesDetection &obstacle_detection) {
	const std::string kWindowTitle = "Guppy camera view";
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
	CraneCameras cameras;
	ObstaclesDetection obstacle_detection;

	std::thread guppy_cam_thread(GuppyCameraThread, std::ref(cameras), std::ref(obstacle_detection));
	guppy_cam_thread.join();

    return 0;
}

