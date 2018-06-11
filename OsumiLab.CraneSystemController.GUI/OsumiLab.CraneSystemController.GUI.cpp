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

void ObstacleAvoidanceDemo();
void MultipleFramesCapture(int step);
void StereoCorrespondance(int steps);
void ObstaclesDetectionDemo();
void SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection);
void MultipleFramesCaptureThread(Crane &crane, CraneSceneCamera &camera, bool &end);

int main() {
	//StereoCorrespondance(8000);
	//Crane c;
	//c.Rope().CalibratePresetValue();
	//c.Rope().MoveTo(1000, 4);
	//c.Rope().ElevateTo(0, 4);
	//c.Rope().Move(Axis::Z, 1000, 4);
	/*c.Rope().CalibratePresetValue();	
	double m = CraneRopeAxis::MeterToEncoderStep(0.3);
	c.Rope().MoveTo(m, 4);*/
	//c.Rope().MoveTo(1000, 4);
	//c.Rope().MoveTo(1000, 4);
	/*c.CoarseAxis().Move(Axis::X, -8000, 0.5);
	c.FineAxis().Move(Axis::Y, -2000, 0.5);
	c.FineAxis().Move(Axis::Y, 2000, 0.5);*/
	//c.Rope().Move(Axis::Z, -10000, 4);
	//StereoCorrespondance();
    return 0;
}

/// <summary>
/// 
/// </summary>
void ObstacleAvoidanceDemo() {
	Crane crane;
	ObstaclesDetection obstacle_detection;
	ObstaclesCorrespondence correspondence;
	obstacle_detection.SetBinaryThreshold(35);

	cv::Mat m1 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(m1);
	obstacle_detection.Detect();
	std::vector<Obstacle> o1 = obstacle_detection.GetObstacles();

	crane.CoarseAxis().Move(Axis::X, -8000, 0.2);

	cv::Mat m2 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(m2);
	obstacle_detection.Detect();
	std::vector<Obstacle> o2 = obstacle_detection.GetObstacles();

	correspondence.SetSamples(o1, o2);
	std::vector<StereoObstacle> o = correspondence.Match();
	if (o.size() > 0) {
		double m = o[0].GetHeight() + 0.1;
		crane.Rope().CalibratePresetValue();
		crane.Rope().ElevateTo(m, 4);
		crane.CoarseAxis().Move(Axis::X, -30000, 0.5);
		crane.Rope().ToGround(4);
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
	cv::namedWindow(kBinaryWinTitle, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kCannyWinTitle, cv::WINDOW_AUTOSIZE);
	cv::namedWindow(kRawWinTitle, cv::WINDOW_AUTOSIZE);

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
		cv::imshow(kBinaryWinTitle, obstacle_detection.GetBinaryFrame());
		cv::imshow(kCannyWinTitle, obstacle_detection.GetCannyFrame());
		cv::imshow(kRawWinTitle, frame_mat);

		if (cv::waitKey(15) >= 0)
			break;

		pre_n_obstacles = n_obstacles;
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

