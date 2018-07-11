#include "stdafx.h"
#include "Demo.h"

/// <summary>
/// 
/// </summary>
/// <param name="steps"></param>
void Demo::SingleCameraStereoCorrespondance(int steps) {
	Crane crane;
	ObstaclesDetection obstacle_detection;
	ObstaclesCorrespondence correspondence;
	obstacle_detection.SetBinaryThreshold(35);

	cv::Mat m1 = crane.RightSceneCamera().GetMat(CV_8UC1);
	obstacle_detection.SetRawFrame(m1);
	obstacle_detection.Detect();
	std::vector<Obstacle> o1 = obstacle_detection.GetObstacles();
	cv::imshow("1", obstacle_detection.GetFrameWithRectangles());
	//crane.CoarseAxis().Move(Axis::X, steps, 0.2);

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
void Demo::DoubleCamerasStereoVision() {
	Crane c;
	ObstaclesDetection od;
	std::vector<Obstacle> oleft, oright;
	ObstaclesCorrespondence correspondence;
	std::vector<StereoObstacle> stereo_obstacles;
	cv::Mat left, right;

	while (1) {
		left = c.LeftSceneCamera().GetMat(CV_8UC1);
		od.SetRawFrame(left);
		od.Detect();
		oleft = od.GetObstacles();
		cv::imshow("left", od.GetFrameWithRectangles());

		right = c.RightSceneCamera().GetMat(CV_8UC1);
		od.SetRawFrame(right);
		od.Detect();
		oright = od.GetObstacles();
		cv::imshow("right", od.GetFrameWithRectangles());

		correspondence.SetSamples(oright, oleft);
		correspondence.Match();
		stereo_obstacles = correspondence.GetStereoObstacles();
		for (auto it = stereo_obstacles.begin(); it != stereo_obstacles.end(); it++) {
			std::cout << (*it).GetHeight() << " m" << std::endl;
		}
		std::cout << std::endl;

		if (cv::waitKey(15) >= 0)
			break;
	}
	cv::destroyAllWindows();
}

/// <summary>
/// 
/// </summary>
/// <param name="camera"></param>
/// <param name="obstacle_detection"></param>
void Demo::SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection) {
	const std::string kWindowTitle = "Raw with obstacles";
	const std::string kBinaryWinTitle = "Binary frame";
	const std::string kCannyWinTitle = "Canny frame";
	const std::string kRawWinTitle = "Raw frame";

	std::vector<Obstacle> obstacles;
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

		obstacles = obstacle_detection.GetObstacles();
		if (obstacle_detection.RopeLoadCollidesWithObstacles())
			std::cout << "Collision detected" << std::endl;

		if (cv::waitKey(15) >= 0)
			break;
	}

	cv::destroyAllWindows();
}

/// <summary>
/// 
/// </summary>
void Demo::SingleCameraObstaclesDetection() {
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
void Demo::RopeSwingRegulation() {
	Crane c;
	RopeSwingRegulator regulator;
	bool stop = false;
	regulator.Regulate(std::ref(c), &stop);
}

/// <summary>
/// 
/// </summary>
void Demo::PrintCamerasRetrievedTime() {
	Crane c;
	FramePtr f;
	auto start = std::chrono::system_clock::now();
	//c.LeftSceneCamera().GetMat(CV_8UC1);
	//c.LeftSceneCamera().GetFrames();
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Left camera retrieve time: " << elapsed.count() << " ms" << std::endl;

	start = std::chrono::system_clock::now();
	c.RightSceneCamera().GetMat(CV_8UC1);
	end = std::chrono::system_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Right camera retrieve time: " << elapsed.count() << " ms" << std::endl;

	start = std::chrono::system_clock::now();
	c.XRopeCamera().GetMat(CV_8UC1);
	end = std::chrono::system_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "XRope camera retrieve time: " << elapsed.count() << " ms" << std::endl;

	start = std::chrono::system_clock::now();
	c.YRopeCamera().GetMat(CV_8UC1);
	end = std::chrono::system_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "YRope camera retrieve time: " << elapsed.count() << " ms" << std::endl;

	std::cout << "Press any key to quit";
	std::cin.get();
}