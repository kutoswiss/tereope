// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneSystemController.h"
//#include "Demo.h"

void LeftFrameGrabber(Crane &c){
	FramePtr f;
	while (true)
	{
		f = c.LeftSceneCamera().GetFrame();
		cv::imshow("Left", CameraHelper::FrameToCVMat(f, CV_8UC1));
		if (cv::waitKey(15) >= 0)
			break;
	}
}

void RightFrameGrabber(Crane &c) {
	FramePtr f;
	while (true)
	{
		f = c.RightSceneCamera().GetFrame();
		cv::imshow("Right", CameraHelper::FrameToCVMat(f, CV_8UC1));
		if (cv::waitKey(15) >= 0)
			break;
	}
}

int main() {
	CraneSystemController controller;
	controller.Execute();
	//Demo::PrintCamerasRetrievedTime();

	//Crane c;
	//std::thread leftcam_thread(LeftFrameGrabber, std::ref(c));
	//std::thread rightcam_thread(RightFrameGrabber, std::ref(c));

	//leftcam_thread.join();
	//rightcam_thread.join();

    return 0;
}
