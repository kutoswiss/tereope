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
#include <string>

void SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection) {
	const std::string kWindowTitle = "Raw with obstacles";
	const std::string kBinaryWinTitle = "Binary frame";
	const std::string kCannyWinTitle = "Canny frame";
	const std::string kRawWinTitle = "Raw frame";


	cv::Mat frame_mat;
	int binary_thr = ObstaclesDetection::kBinaryThresholdValue;
	int n_obstacles, pre_n_obstacles = 0;
	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kBinaryWinTitle, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kCannyWinTitle, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kRawWinTitle, cv::WINDOW_AUTOSIZE);

	//cv::createTrackbar("Binary Thr.", kWindowTitle, &binary_thr, 255, NULL);
	int i = 0;
	
	while (1) {
		frame_mat = camera.GetMat(CV_8UC1);
		obstacle_detection.SetBinaryThreshold(binary_thr);
		obstacle_detection.SetRawFrame(frame_mat);
		n_obstacles = obstacle_detection.Detect();

		if (n_obstacles != pre_n_obstacles)
			obstacle_detection.PrintDetect();

		cv::imshow(kWindowTitle, obstacle_detection.GetFrameWithRectangles());
		cv::imshow(kBinaryWinTitle, obstacle_detection.GetBinaryFrame());
		cv::imshow(kCannyWinTitle, obstacle_detection.GetCannyFrame());
		cv::imshow(kRawWinTitle, frame_mat);
		
		if (cv::waitKey(15) >= 0)
			break;

		pre_n_obstacles = n_obstacles;
	}

	cv::destroyAllWindows();
}

void StereoDetection(cv::Mat left, cv::Mat right) {

}
 
int main() {
	/*Crane crane;
	ObstaclesDetection obstacle_detection;
	obstacle_detection.SetBinaryThreshold(25);

	cv::Mat img = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(img);
	obstacle_detection.Detect();
	cv::namedWindow("1", cv::WINDOW_AUTOSIZE);
	cv::imshow("1", obstacle_detection.GetFrameWithRectangles());

	crane.CoarseAxis().Move(Axis::X, -10000);

	cv::Mat img2 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(img2);
	obstacle_detection.Detect();
	cv::namedWindow("2", cv::WINDOW_AUTOSIZE);
	cv::imshow("2", obstacle_detection.GetFrameWithRectangles());

	cv::waitKey(0);
	cv::destroyWindow("1");
	cv::destroyWindow("2");
	
	system("pause");*/

	Crane crane;
	
	ObstaclesDetection obstacle_detection;
	std::thread scene_cam_thread(
		SceneCameraThread,
		std::ref(crane.RightSceneCamera()), 
		std::ref(obstacle_detection));
	scene_cam_thread.join();

	//cv::namedWindow("Left", cv::WINDOW_AUTOSIZE);
	//obstacle_detection.SetRawFrame(left);
	//obstacle_detection.Detect();
	//cv::imshow("Left", obstacle_detection.GetFrameWithRectangles());
	//cv::imwrite("left.png", obstacle_detection.GetFrameWithRectangles());

	//cv::namedWindow("Right", cv::WINDOW_AUTOSIZE);
	//obstacle_detection.SetRawFrame(right);
	//obstacle_detection.Detect();
	//cv::imshow("Right", obstacle_detection.GetFrameWithRectangles());
	//cv::imwrite("right.png", obstacle_detection.GetFrameWithRectangles());

	//cv::waitKey(0);
	//cv::destroyWindow("Left");
	//cv::destroyWindow("Right");

    return 0;
}

