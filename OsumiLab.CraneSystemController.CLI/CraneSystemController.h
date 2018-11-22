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
	// Enum
	enum State {
		MOVING,
		STOP,
		AVOIDING_OBSTACLES
	};

	// Ctor/Dtor
	CraneSystemController();
	~CraneSystemController();

	// Public method
	void Execute();

private:
	// Private method (task)
	void CommandTask(Crane &c);
	void RopeSwingingRegulationTask(Crane &c);
	void ObstacleAvoidanceTask(Crane &c);
	void LeftCamCollisionDetectionTask(Crane &c);
	void RightCamCollisionDetectionTask(Crane &c);
	void XRopeSwingingRegulationTask(Crane &c);
	void YRopeSwingingRegulationTask(Crane &c);

	void RetrieveStereoObstacles();
	void AvoidObstacles();
	void Overpass(int x_steps, int y_steps);
	double GetHeighestCollidedObstacleValue();
	double GetHeighestObstacleValue();

	// Private members
	std::unique_ptr<std::thread> _cmd_task;
	std::unique_ptr<std::thread> _collision_detection_task;
	std::unique_ptr<std::thread> _leftcam_collision_detection_task;
	std::unique_ptr<std::thread> _rightcam_collision_detection_task;
	std::unique_ptr<std::thread> _rope_regulation_task;
	std::unique_ptr<std::thread> _xrope_regulation_task;
	std::unique_ptr<std::thread> _yrope_regulation_task;
	std::unique_ptr<std::thread> _obstacle_avoidance_task;

	std::mutex _m;
	std::condition_variable _cv_collisiondetection_done;
	std::condition_variable _cv_ropeswinging_done;
	std::condition_variable _cv_avoidance_done;
	std::condition_variable _cv_obstacle_detected;

	ObstaclesDetection _left_detector;
	ObstaclesDetection _right_detector;

	ObstaclesCorrespondence _correspondence;
	std::vector<StereoObstacle> _obstacles;

	Crane _crane;
	State _state;

	cv::Mat _leftcam;
	cv::Mat _rightcam;
	cv::Mat _xcam; // mdr
	cv::Mat _ycam;

	int _x_distance = 0;
	int _y_distance = 0;
	double _rope_height = 0;

	RopeSwingRegulator _regulator;
	bool _general_stop_signal;

	int _left_ropearea_collide = 0;
	int _right_ropearea_collide = 0;
	double _rope_meter = 0;
};
