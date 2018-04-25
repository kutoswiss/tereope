// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneCameras.h"
#include "ObstaclesDetection.h"
#include "CameraHelper.h"
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>Å@
#include <condition_variable>Å@

// Sync. primitives
std::mutex mutex_obsdetection;
std::mutex mutex_capturedone;
std::condition_variable cvar_capturedone;


void GuppyCameraThread(CraneCameras &cameras, ObstaclesDetection &obstacle_detection) {
	const std::string kWindowTitle = "Guppy camera view";
	FramePtr frame;
	cv::Mat frame_mat;

	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	while(1) {
		frame = cameras.GetFrame(0);
		frame_mat = CameraHelper::FrameToCVMat(frame, CV_8UC1);
		cv::cvtColor(frame_mat, frame_mat, CV_BayerBG2RGB);

		mutex_obsdetection.lock();
		obstacle_detection.SetRawFrame(frame_mat);
		obstacle_detection.Detect();
		cv::imshow(kWindowTitle, obstacle_detection.GetFrameWithRectangles(true, true, true));
		mutex_obsdetection.unlock(); 
		cvar_capturedone.notify_all(); // Initialization done
		
		if (cv::waitKey(15) >= 0) 
			break;
	}
	cv::destroyWindow(kWindowTitle);
}

void GuppyCannyThread(ObstaclesDetection &obstacle_detection) {
	// Wait until the initialization of GuppyCameraThread
	std::unique_lock<std::mutex> locker(mutex_capturedone);
	cvar_capturedone.wait(locker);
	locker.unlock();

	const std::string kWindowTitle = "Guppy Canny view";
	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	while(1) {
		mutex_obsdetection.lock();
		cv::imshow(kWindowTitle, obstacle_detection.GetCannyFrame());
		mutex_obsdetection.unlock();

		if (cv::waitKey(15) >= 0)
			break;
	}
	cv::destroyWindow(kWindowTitle);
}

void GuppyBinaryThread(ObstaclesDetection &obstacle_detection) {
	// Wait until the initialization of GuppyCameraThread
	std::unique_lock<std::mutex> locker(mutex_capturedone);
	cvar_capturedone.wait(locker);
	locker.unlock();

	const std::string kWindowTitle = "Guppy Binary view";
	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	while (1) {
		mutex_obsdetection.lock();
		cv::imshow(kWindowTitle, obstacle_detection.GetBinaryFrame());
		mutex_obsdetection.unlock();

		if (cv::waitKey(15) >= 0)
			break;
	}
	cv::destroyWindow(kWindowTitle);
}


int main() {
	CraneCameras cameras;
	ObstaclesDetection obstacle_detection;

	std::thread guppy_cam_thread(GuppyCameraThread, std::ref(cameras), std::ref(obstacle_detection));
	std::thread guppy_canny_thread(GuppyCannyThread, std::ref(obstacle_detection));
	std::thread guppy_binary_thread(GuppyBinaryThread, std::ref(obstacle_detection));

	guppy_cam_thread.join();
	guppy_canny_thread.join();
	guppy_binary_thread.join();

    return 0;
}
