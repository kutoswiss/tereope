// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ObstaclesDetection.h"
#include "ObstaclesCorrespondence.h"
#include "Crane.h"

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>Å@
#include <condition_variable>Å@
#include <chrono>    
#include <string>

void ObstacleAvoidanceDemo(int steps);
void MultipleFramesCapture(int step);
void StereoCorrespondance(int steps);
void ObstaclesDetectionDemo();
void SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection);
void MultipleFramesCaptureThread(Crane &crane, CraneSceneCamera &camera, bool &end);
void CLIController();
void ObstacleCollisionDetectionThread(Crane &crane, ObstaclesDetection &detector);

int main() {
	//Crane c;
	//c.Rope().Move(Axis::Z, -1000, 4);
	//c.Rope().CalibratePresetValue();
	//ObstaclesDetectionDemo();
	CLIController();
    return 0;
}

void CLIController() {
	Crane c;
	c.Rope().CalibratePresetValue();
	ObstaclesDetection detector;
	detector.SetBinaryThreshold(35);
	int x_val = 0;
	int y_val = 0;
	//std::thread collision_thread(
	//	ObstacleCollisionDetectionThread,
	//	std::ref(c),
	//	std::ref(detector));

	std::string input;
	while (true) {
		std::cout << "> ";
		std::cin >> input;
		if (input == "x") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			c.CoarseAxis().MoveThread(Axis::X, x_val, 0.5);
		}
		else if (input == "y") {
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.CoarseAxis().MoveThread(Axis::Y, y_val, 0.2);
		}
		else if (input == "xy") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.CoarseAxis().MoveThread(Axis::X, x_val, 0.5);
			c.CoarseAxis().MoveThread(Axis::Y, y_val, 0.5);
		}
		else if (input == "stop") {
			c.CoarseAxis().Stop(Axis::X);
			std::cout << c.CoarseAxis().GetCntValue(Axis::X) << std::endl;
		}
		else if (input == "quit")
			break;
	}

	//collision_thread.join();
}


/// <summary>
/// 
/// </summary>
void ObstacleAvoidanceDemo(int steps) {
	Crane crane;
	ObstaclesDetection obstacle_detection;
	ObstaclesCorrespondence correspondence;
	obstacle_detection.SetBinaryThreshold(35);

	cv::Mat m1 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(m1);
	obstacle_detection.Detect();
	std::vector<Obstacle> o1 = obstacle_detection.GetObstacles();

	crane.CoarseAxis().MoveX(steps, 0.2);

	cv::Mat m2 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(m2);
	obstacle_detection.Detect();
	std::vector<Obstacle> o2 = obstacle_detection.GetObstacles();

	correspondence.SetSamples(o1, o2);
	std::vector<StereoObstacle> o = correspondence.Match();
	if (o.size() > 0) {
		double m = o[0].GetHeight() + 0.05;
		crane.Rope().ElevateTo(m);
		crane.CoarseAxis().MoveX(-30000, 0.5);
		crane.Rope().ToGround();
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
void MultipleFramesCapture(int step) {
	bool end = false;
	Crane crane;

	std::thread t(
		MultipleFramesCaptureThread,
		std::ref(crane),
		std::ref(crane.RightSceneCamera()),
		std::ref(end));

	crane.CoarseAxis().Move(Axis::X, step);
	end = true;
	t.join();
}

/// <summary>
/// 
/// </summary>
void StereoCorrespondance(int steps) {
	Crane crane;
	ObstaclesDetection obstacle_detection;
	ObstaclesCorrespondence correspondence;
	obstacle_detection.SetBinaryThreshold(35);

	cv::Mat m1 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(m1);
	obstacle_detection.Detect();
	std::vector<Obstacle> o1 = obstacle_detection.GetObstacles();
	cv::imshow("1", obstacle_detection.GetFrameWithRectangles());
	crane.CoarseAxis().Move(Axis::X, steps, 0.2);

	cv::Mat m2 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(m2);
	obstacle_detection.Detect();
	std::vector<Obstacle> o2 = obstacle_detection.GetObstacles();
	cv::imshow("2", obstacle_detection.GetFrameWithRectangles());

	correspondence.SetSamples(o1, o2);
	correspondence.Match();
	correspondence.PrintMatchedObstacles();

	cv::waitKey(0);
	cv::destroyWindow("Left");
	cv::destroyWindow("Right");
}

/// <summary>
/// 
/// </summary>
void ObstaclesDetectionDemo() {
	Crane crane;
	ObstaclesDetection obstacle_detection;
	obstacle_detection.SetBinaryThreshold(35);

	std::thread scene_cam_thread(
	SceneCameraThread,
	std::ref(crane.RightSceneCamera()),
	std::ref(obstacle_detection));
	scene_cam_thread.join();
}

/// <summary>
/// 
/// </summary>
/// <param name="crane"></param>
/// <param name="detector"></param>
void ObstacleCollisionDetectionThread(Crane &crane, ObstaclesDetection &detector) {

	while (true) {
		cv::Mat m1 = crane.RightSceneCamera().GetMat(CV_8UC1);
		detector.SetRawFrame(m1);
		detector.Detect();
		detector.GetFrameWithRectangles();
		if (detector.IsCollided()) {
			crane.CoarseAxis().Stop(Axis::X);
			std::cout << "Collision detected." << std::endl;
			
			cv::Mat m1 = crane.RightSceneCamera().GetMat(CV_8UC1);
			detector.SetRawFrame(m1);
			detector.Detect();
			std::vector<Obstacle> o1 = detector.GetObstacles();
			cv::namedWindow("1", cv::WINDOW_AUTOSIZE);
			cv::imshow("1", detector.GetFrameWithRectangles());

			crane.CoarseAxis().MoveX(-8000, 0.2);

			cv::Mat m2 = crane.RightSceneCamera().GetMat(CV_8UC1);
			detector.SetRawFrame(m2);
			detector.Detect();
			std::vector<Obstacle> o2 = detector.GetObstacles();
			cv::namedWindow("2", cv::WINDOW_AUTOSIZE);
			cv::imshow("2", detector.GetFrameWithRectangles());

			ObstaclesCorrespondence correspondence;
			correspondence.SetSamples(o1, o2);
			std::vector<StereoObstacle> o = correspondence.Match();
			cv::waitKey(0);
			cv::destroyAllWindows();

			if (o.size() > 0) {
				double m = o[0].GetHeight() + 0.05;
				crane.Rope().ElevateTo(m);
				crane.CoarseAxis().MoveX(-30000, 0.5);
				crane.Rope().ToGround();
			}
			break;
		}
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="camera"></param>
/// <param name="obstacle_detection"></param>
void SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection) {
	const std::string kWindowTitle = "Raw with obstacles";
	const std::string kBinaryWinTitle = "Binary frame";
	const std::string kCannyWinTitle = "Canny frame";
	const std::string kRawWinTitle = "Raw frame";


	cv::Mat frame_mat;
	int binary_thr = ObstaclesDetection::kBinaryThresholdValue;
	int n_obstacles, pre_n_obstacles = 0;
	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);

	cv::createTrackbar("Binary Thr.", kWindowTitle, &binary_thr, 255, NULL);
	int i = 0;

	while (1) {
		frame_mat = camera.GetMat(CV_8UC1);
		obstacle_detection.SetRawFrame(frame_mat);
		obstacle_detection.SetBinaryThreshold(binary_thr);

		n_obstacles = obstacle_detection.Detect();
		if (n_obstacles != pre_n_obstacles)
			obstacle_detection.PrintDetect();
		
		cv::imshow(kWindowTitle, obstacle_detection.GetFrameWithRectangles());
		pre_n_obstacles = n_obstacles;
		if (cv::waitKey(15) >= 0) 
			break;
	}

	cv::destroyAllWindows();
}

void MultipleFramesCaptureThread(Crane &crane, CraneSceneCamera &camera, bool &end) {
	const std::string kWindowTitle = "Frames";
	cv::Mat frame_mat;
	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);

	int i = 0;
	while (!end) {
		std::stringstream ss;
		ss << "dai/img" << i++ << ".png";
		frame_mat = camera.GetMat(CV_8UC1);
		cv::imshow(kWindowTitle, frame_mat);
		cv::imwrite(ss.str(), frame_mat);
		cv::waitKey(1);
	}
	cv::destroyAllWindows();
}

