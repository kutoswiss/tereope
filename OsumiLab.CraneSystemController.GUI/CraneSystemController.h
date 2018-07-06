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
	CraneSystemController();
	~CraneSystemController();

	void Execute();

private:
	void CommandTask(Crane &c);
	void CollisionDetectionTask(Crane &c);
	void RopeSwingingRegulationTask(Crane &c);

	std::unique_ptr<std::thread> _cmd_task;
	std::unique_ptr<std::thread> _rope_regulation_task;
	std::unique_ptr<std::thread> _collision_detection_task;
	bool _general_stop_signal;
};

