// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CraneCameras.h"
#include "ObstaclesDetection.h"
#include "CameraHelper.h"
#include "Caio.h"
#include "CCnt.h"
#include "Crane.h"
#include "AbstractCraneAxis.h"
#include "CraneSceneCamera.h"

#include <vector>
#include <iostream>
#include <thread>
#include <mutex>Å@
#include <condition_variable>Å@
#include <chrono>    

// Sync. primitives
std::mutex mutex_obsdetection;
std::mutex mutex_capturedone;
std::condition_variable cvar_capturedone;

void SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection) {
	const std::string kWindowTitle = "Scene camera view";
	FramePtr frame;
	cv::Mat frame_mat;
	int binary_thr = ObstaclesDetection::kBinaryThresholdValue;

	cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Binary Thr.", kWindowTitle, &binary_thr, 255, NULL);

	while (1) {
		frame = camera.GetFrame();
		frame_mat = CameraHelper::FrameToCVMat(frame, CV_8UC1);
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

/// <summary>
/// Process to show frame of Guppy cameras with obstacles detection
/// </summary>
/// <param name="cameras"></param>
/// <param name="obstacle_detection"></param>
void GuppyCameraThread(CraneCameras &cameras, ObstaclesDetection &obstacle_detection) {
    const std::string kWindowTitle = "Guppy camera view";
    FramePtr frame;
    cv::Mat frame_mat;
	int binary_threshold = ObstaclesDetection::kBinaryThresholdValue;

    cv::namedWindow(kWindowTitle, cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("Binary Thr.", kWindowTitle, &binary_threshold, 255, NULL);
    while(1) {
        frame = cameras.GetFrame(0);
        frame_mat = CameraHelper::FrameToCVMat(frame, CV_8UC1);
        cv::cvtColor(frame_mat, frame_mat, CV_BayerBG2RGB);

        mutex_obsdetection.lock();
		obstacle_detection.SetBinaryThreshold(binary_threshold);
        obstacle_detection.SetRawFrame(frame_mat);
        obstacle_detection.Detect();
        cv::imshow(kWindowTitle, obstacle_detection.GetFrameWithRectangles());
        mutex_obsdetection.unlock(); 
        cvar_capturedone.notify_all(); // Initialization done, notify all threads
        
        if (cv::waitKey(15) >= 0) 
            break;
    }
    cv::destroyWindow(kWindowTitle);
}

/// <summary>
/// Process to show frames after Canny operation
/// </summary>
/// <param name="obstacle_detection"></param>
void GuppyCannyThread(ObstaclesDetection &obstacle_detection) {
    // Wait until the initialization of GuppyCameraThread
    {
        std::unique_lock<std::mutex> locker(mutex_capturedone);
        cvar_capturedone.wait(locker);
        locker.unlock();
    }

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

/// <summary>
/// Process to show frames after binary operation
/// </summary>
/// <param name="obstacle_detection"></param>
void GuppyBinaryThread(ObstaclesDetection &obstacle_detection) {
    // Wait until the initialization of GuppyCameraThread
    {
        std::unique_lock<std::mutex> locker(mutex_capturedone);
        cvar_capturedone.wait(locker);
        locker.unlock();
    }

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

void CraneTest() {
	//const int kAioChannel = 16;
	//const int kCntChannel = 8;

	//short aio_id, cnt_id;
	//float ao_data[kAioChannel];
	//short channel_start[kCntChannel];
	//unsigned long preset_data[kCntChannel];
	//DWORD count = 0;

	//AioInit("AIO000", &aio_id);
	//AioResetDevice(aio_id);
	//AioSetAoRangeAll(aio_id, PM10);

	//memset(ao_data, 0.0, sizeof(float) * kAioChannel);
	//AioMultiAoEx(aio_id, kAioChannel, &ao_data[0]);
	//for (int i = 7; i <= 11; i++)
	//	ao_data[i] = 5.0;
	//AioMultiAoEx(aio_id, kAioChannel, &ao_data[0]);

	//CntInit("CNT000", &cnt_id);
	//for (int channel = 0; channel < kCntChannel; channel++) {
	//	CntSetZMode(cnt_id, channel, CNT_ZPHASE_NOT_USE);
	//	CntSetZLogic(cnt_id, channel, CNT_ZLOGIC_POSITIVE);
	//	CntSelectChannelSignal(cnt_id, channel, CNT_ZLOGIC_POSITIVE);
	//	CntSetCountDirection(cnt_id, channel, CNT_DIR_UP);
	//	CntSetOperationMode(cnt_id, channel, CNT_MODE_2PHASE, CNT_MUL_X4, CNT_CLR_ASYNC);
	//	CntSetDigitalFilter(cnt_id, channel, 0);
	//	channel_start[channel] = channel;
	//	preset_data[channel] = 2000000; 
	//}
	//channel_start[0] = 3;
	//CntPreset(cnt_id, channel_start, kCntChannel, preset_data);
	//CntStartCount(cnt_id, channel_start, 1);
	//AioSingleAoEx(aio_id, 1, 0.5);
	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//CntStopCount(cnt_id, channel_start, 1);
	//CntReadCount(cnt_id, channel_start, 1, &count);

	//// Set to 0[V] on AIO
	//memset(ao_data, 0.0, sizeof(float) * kAioChannel);
	//AioMultiAoEx(aio_id, kAioChannel, &ao_data[0]);

	//AioExit(aio_id);
	//CntExit(cnt_id);
}

 
int main() {
	{
		//Crane crane;
		//crane.Rope().Move(Axis::Z, 170000);
		//crane.FineAxis().Move(Axis::X, -2500);
		//crane.CoarseAxis().Move(Axis::X, 3000);
		//crane.CoarseAxis().Move(Axis::Y, -1500);
		//crane.CoarseAxis().Move(Axis::X, +500);
		//crane.CoarseAxis().Move(Axis::Y, +1500);

		//crane.FineAxis().Move(Axis::Y, -500);
		//crane.FineAxis().Move(Axis::Y, 500);
	}

	//CraneTest();

    /*CraneCameras cameras;
    ObstaclesDetection obstacle_detection;

    std::thread guppy_cam_thread(GuppyCameraThread, std::ref(cameras), std::ref(obstacle_detection));
    std::thread guppy_canny_thread(GuppyCannyThread, std::ref(obstacle_detection));
    std::thread guppy_binary_thread(GuppyBinaryThread, std::ref(obstacle_detection));

    guppy_cam_thread.join();
    guppy_canny_thread.join();
    guppy_binary_thread.join();*/

	{
		Crane crane;
		ObstaclesDetection obstacle_detection;
		std::thread scene_cam_thread(SceneCameraThread, std::ref(crane.RightSceneCamera()), std::ref(obstacle_detection));

		crane.CoarseAxis().Move(Axis::X, 3000);
		//crane.Rope().Move(Axis::Z, -50000);

		scene_cam_thread.join();
	}

    return 0;
}
