#pragma once
#include "ObstaclesDetection.h"
#include "ObstaclesCorrespondence.h"
#include "Crane.h"
#include "RopeSwingRegulator.h"
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>Å@
#include <condition_variable>Å@
#include <chrono>    
#include <string>

class CraneSystemController
{
public:
	// Ctor/Dtor
	CraneSystemController();
	~CraneSystemController();

	// Public method
	void Execute();

private:
	// Private method
	void CommandTask(Crane &c);
	void RopeSwingingRegulationTask(Crane &c);
	void ObstacleAvoidanceTask(Crane &c);
	void LeftCamCollisionDetectionTask(Crane &c);
	void RightCamCollisionDetectionTask(Crane &c);

	// Private members
	std::unique_ptr<std::thread> _cmd_task;
	std::unique_ptr<std::thread> _rope_regulation_task;
	std::unique_ptr<std::thread> _leftcam_collision_detection_task;
	std::unique_ptr<std::thread> _rightcam_collision_detection_task;
	std::unique_ptr<std::thread> _obstacle_avoidance_task;

	std::mutex _m;
	std::condition_variable _cv_collisiondetection_done;
	std::condition_variable _cv_ropeswinging_done;

	cv::Mat _leftcam;
	cv::Mat _rightcam;
	cv::Mat _xcam; // mdr
	cv::Mat _ycam;

	bool _general_stop_signal;
};
