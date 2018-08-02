// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneSystemController.h"
//#include "Demo.h"

int main() {
	CraneSystemController ctrl;
	ctrl.Execute();

	//Crane crane;
	//cv::Mat right = crane.RightSceneCamera().GetMat(CV_8UC1);
	//cv::Mat left = crane.LeftSceneCamera().GetMat(CV_8UC1);

	//ObstaclesDetection left_det, right_det;

	//left_det.SetRawFrame(left);
	//left_det.Detect();

	//right_det.SetRawFrame(right);
	//right_det.Detect();

	//std::vector<Obstacle> right_obs = right_det.GetObstacles();
	//std::vector<Obstacle> left_obs = left_det.GetObstacles();

	//ObstaclesCorrespondence correspondence;
	//correspondence.SetSamples(left_obs, right_obs);
	//correspondence.Match();
	//std::vector<StereoObstacle> so = correspondence.GetStereoObstacles();
	//correspondence.PrintMatchedObstacles();


	//cv::imshow("Left", left_det.GetFrameWithRectangles());
	//cv::imshow("Right", right_det.GetFrameWithRectangles());

	//cv::waitKey();
    return 0;
}
