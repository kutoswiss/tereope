// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneSystemController.h"
//#include "Demo.h"

int main() {
	CraneSystemController ctrl;
	ctrl.Execute();

	// Example for Tsuchiya
	//Crane crane;
	//ObstaclesDetection left_det, right_det;
	//ObstaclesCorrespondence correspondence;

	// Get frames from scene cameras
	//cv::Mat right = crane.RightSceneCamera().GetMat(CV_8UC1);
	//cv::Mat left = crane.LeftSceneCamera().GetMat(CV_8UC1);

	// Set frame for left detection
	//left_det.SetRawFrame(left);
	//left_det.Detect();

	// Set frame for right detection
	//right_det.SetRawFrame(right);
	//right_det.Detect();

	// Retreive obstacles from both images
	//std::vector<Obstacle> right_obs = right_det.GetObstacles();
	//std::vector<Obstacle> left_obs = left_det.GetObstacles();

	// Get stereo obstacles
	//correspondence.SetSamples(left_obs, right_obs);
	//correspondence.Match();
	//std::vector<StereoObstacle> so = correspondence.GetStereoObstacles();
	//correspondence.PrintMatchedObstacles();

	// Show images
	//cv::imshow("Left", left_det.GetFrameWithRectangles());
	//cv::imshow("Right", right_det.GetFrameWithRectangles());
	//cv::waitKey();

    return 0;
}
