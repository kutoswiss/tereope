// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ObstaclesDetection.h"
#include "Crane.h"

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>Å@
#include <condition_variable>Å@
#include <chrono>    

void SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection) {
	const std::string kWindowTitle = "Scene camera view";
	cv::Mat frame_mat;
	int binary_thr = ObstaclesDetection::kBinaryThresholdValue;

	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Binary Thr.", kWindowTitle, &binary_thr, 255, NULL);

	while (1) {
		frame_mat = camera.GetMat(CV_8UC1);
		cv::cvtColor(frame_mat, frame_mat, CV_BayerBG2RGB);

		obstacle_detection.SetBinaryThreshold(binary_thr);
		obstacle_detection.SetRawFrame(frame_mat);
		obstacle_detection.Detect();
		cv::imshow(kWindowTitle, obstacle_detection.GetFrameWithRectangles());

		if (cv::waitKey(15) >= 0)
			break;
	}

	cv::destroyWindow(kWindowTitle);
}
 
int main() {
	Crane crane;
	ObstaclesDetection obstacle_detection;

	cv::Mat right = crane.RightSceneCamera().GetMat(CV_8UC1);
	//std::thread scene_cam_thread(SceneCameraThread, std::ref(crane.RightSceneCamera()), std::ref(obstacle_detection));
	crane.CoarseAxis().Move(Axis::X, -30000);
	cv::Mat left = crane.RightSceneCamera().GetMat(CV_8UC1);
	//scene_cam_thread.join();

	cv::namedWindow("Left", cv::WINDOW_AUTOSIZE);
	obstacle_detection.SetRawFrame(left);
	obstacle_detection.Detect();
	cv::imshow("Left", obstacle_detection.GetFrameWithRectangles());
	cv::imwrite("left.png", obstacle_detection.GetFrameWithRectangles());

	cv::namedWindow("Right", cv::WINDOW_AUTOSIZE);
	obstacle_detection.SetRawFrame(right);
	obstacle_detection.Detect();
	cv::imshow("Right", obstacle_detection.GetFrameWithRectangles());
	cv::imwrite("right.png", obstacle_detection.GetFrameWithRectangles());

	cv::waitKey(0);
	cv::destroyWindow("Left");
	cv::destroyWindow("Right");

    return 0;
}
