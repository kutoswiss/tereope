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
	void CollisionDetectionTask(Crane &c);
	void RopeSwingingRegulationTask(Crane &c);
	void ObstacleAvoidanceTask(Crane &c);

	// Private members
	std::unique_ptr<std::thread> _cmd_task;
	std::unique_ptr<std::thread> _rope_regulation_task;
	std::unique_ptr<std::thread> _collision_detection_task;
	std::unique_ptr<std::thread> _obstacle_avoidance_task;

	std::mutex _mtx_avoided;
	std::mutex _mtx_collide;
	std::condition_variable _cv_collide;
	std::condition_variable _cv_avoided;

	bool _general_stop_signal;
};
