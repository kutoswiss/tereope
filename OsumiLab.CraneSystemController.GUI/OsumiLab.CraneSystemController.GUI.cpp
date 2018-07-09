// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneSystemController.h"
//#include "Demo.h"

//void MultipleFramesCapture(int step);
//void MultipleFramesCaptureThread(Crane &crane, CraneSceneCamera &camera, bool &end);

int main() {
	CraneSystemController controller;
	controller.Execute();
    return 0;
}

//
//void MultipleFramesCapture(int step) {
//	bool end = false;
//	Crane crane;
//
//	std::thread t(
//		MultipleFramesCaptureThread,
//		std::ref(crane),
//		std::ref(crane.RightSceneCamera()),
//		std::ref(end));
//
//	crane.CoarseAxis().Move(Axis::X, step, 0.5);
//	end = true;
//	t.join();
//}
//
//void MultipleFramesCaptureThread(Crane &crane, CraneSceneCamera &camera, bool &end) {
//	const std::string kWindowTitle = "Frames";
//	cv::Mat frame_mat;
//	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
//
//	int i = 0;
//	while (!end) {
//		std::stringstream ss;
//		ss << "dai/img" << i++ << ".png";
//		frame_mat = camera.GetMat(CV_8UC1);
//		cv::imshow(kWindowTitle, frame_mat);
//		cv::imwrite(ss.str(), frame_mat);
//		cv::waitKey(1);
//	}
//	cv::destroyAllWindows();
//}

