// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneSystemController.h"
//#include "Demo.h"
#include "CraneCameras.h"

int main() {

	//Crane c;
	//c.RopeMS()->Move("Z", 2000, 4);
	//c.RopeMS()->CalibratePresetValue();
	CraneSystemController controller;
	controller.Execute();

	//bool _general_stop_signal = false;
	//Crane c;
	//RopeSwingRegulator regulator;
	//regulator.Regulate(std::ref(c), &_general_stop_signal);
	//Crane c;

	//while (true)
	//{
	//	cv::imshow("0", c.LeftSceneCamera().GetMat(CV_8UC1));
	//	cv::imshow("1", c.RightSceneCamera().GetMat(CV_8UC1));
	//	cv::imshow("2", c.XRopeCamera().GetMat(CV_8UC1));
	//	cv::imshow("3", c.YRopeCamera().GetMat(CV_8UC1));

	//	if (cv::waitKey(15) >= 0)
	//		break;
	//}
	//cv::destroyAllWindows();
    return 0;
}
