#include "stdafx.h"
#include "CraneSystemController.h"

/// <summary>
/// 
/// </summary>
CraneSystemController::CraneSystemController() {
	_general_stop_signal = false;
	_left_detector.SetRopeLoadAreaOrigin(cv::Point(350, 325));
	_state = State::STOP;
}

/// <summary>
/// 
/// </summary>
CraneSystemController::~CraneSystemController() {
}

/// <summary>
/// 
/// </summary>
void CraneSystemController::Execute() {
	// Command task thread
	_cmd_task = std::make_unique<std::thread>(
		&CraneSystemController::CommandTask,
		this, 
		std::ref(_crane));

	// Left camera collsion detection task
	_leftcam_collision_detection_task = std::make_unique<std::thread>(
		&CraneSystemController::LeftCamCollisionDetectionTask,
		this,
		std::ref(_crane));

	// Right camera collision detection task
	_rightcam_collision_detection_task = std::make_unique<std::thread>(
		&CraneSystemController::RightCamCollisionDetectionTask,
		this,
		std::ref(_crane));

	// Obstacle avoidance task
	std::this_thread::sleep_for(500ms);
	_obstacle_avoidance_task = std::make_unique<std::thread>(
		&CraneSystemController::ObstacleAvoidanceTask,
		this,
		std::ref(_crane));

	// Y rope regulation task
	//_yrope_regulation_task = std::make_unique<std::thread>(
	//	&CraneSystemController::YRopeSwingingRegulationTask,
	//	this,
	//	std::ref(_crane));

	// X rope regulation task
	//_xrope_regulation_task = std::make_unique<std::thread>(
	//	&CraneSystemController::XRopeSwingingRegulationTask, 
	//	this, 
	//	std::ref(_crane));

	// Join tasks
	_cmd_task->join();
	_leftcam_collision_detection_task->join();
	_rightcam_collision_detection_task->join();
	_obstacle_avoidance_task->join();
	//_yrope_regulation_task->join();
	//_xrope_regulation_task->join();
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::LeftCamCollisionDetectionTask(Crane &c) {
	bool collide = false;

	while (_general_stop_signal == false) {
		_leftcam = c.LeftSceneCamera().GetMat(CV_8UC1);
		_left_detector.SetRawFrame(_leftcam);
		_left_detector.Detect();
		_left_ropearea_collide = _left_detector.RopeAreaCollidesWithObstacles();
		collide = _left_detector.RopeLoadCollidesWithObstacles();

		if ((_state == State::MOVING) && collide) {
			c.Coarse()->Halt();
			_cv_obstacle_detected.notify_one();
		}

		cv::imshow("Left scene camera", _left_detector.GetFrameWithRectangles());
		if (cv::waitKey(50) >= 0)
			break;
	}
	_cv_obstacle_detected.notify_one();
	cv::destroyAllWindows();
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::RightCamCollisionDetectionTask(Crane &c) {
	bool collide = false;

	while (_general_stop_signal == false) {
		_rightcam = c.RightSceneCamera().GetMat(CV_8UC1);
		_right_detector.SetRawFrame(_rightcam);
		_right_detector.Detect();
		_right_ropearea_collide = _right_detector.RopeAreaCollidesWithObstacles();
		collide = _right_detector.RopeLoadCollidesWithObstacles();

		if ((_state == State::MOVING) && collide) {
			c.Coarse()->Halt();
			_cv_obstacle_detected.notify_one();
		}

		cv::imshow("Right scene camera", _right_detector.GetFrameWithRectangles());
		if (cv::waitKey(50) >= 0)
			break;
	}

	_cv_obstacle_detected.notify_one();
	cv::destroyAllWindows();
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::XRopeSwingingRegulationTask(Crane &c) {
	_regulator.RegulateX(c, &_general_stop_signal);
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::YRopeSwingingRegulationTask(Crane &c) {
	_regulator.RegulateY(c, &_general_stop_signal);
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::RopeSwingingRegulationTask(Crane &c) {
	RopeSwingRegulator regulator;
	regulator.Regulate(std::ref(c), &_general_stop_signal);
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::ObstacleAvoidanceTask(Crane &c) {
	std::unique_lock<std::mutex> lock(_m);
	while (_general_stop_signal == false) {
		_cv_obstacle_detected.wait(lock);
		if (_general_stop_signal == true)
			break;

		_state = State::AVOIDING_OBSTACLES;
		this->RetrieveStereoObstacles();
		this->AvoidObstacles();
		_cv_avoidance_done.notify_all();
	}
}


/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::CommandTask(Crane &c) {
	std::string input;

	while (true) {
		std::cout << "> ";
		std::cin >> input;
		_x_distance = 0;
		_y_distance = 0;
		_rope_height = 0.0;

		if (input == "x") {
			std::cout << "> Enter X value: ";
			std::cin >> _x_distance;
			c.Coarse()->XThread(_x_distance, 0.3);
			_state = State::MOVING;
		}
		else if (input == "y") {
			std::cout << "> Enter Y value: ";
			std::cin >> _y_distance;
			c.Coarse()->YThread(_y_distance, 0.3);
			_state == State::MOVING;
		}
		else if (input == "xy") {
			std::cout << "> Enter X value: ";
			std::cin >> _x_distance;
			std::cout << "> Enter Y value: ";
			std::cin >> _y_distance;
			c.Coarse()->XThread(_x_distance, 0.3);
			c.Coarse()->YThread(_y_distance, 0.3);
			_state == State::MOVING;
		}
		else if (input == "rope") {
			std::cout << "> Enter rope height: ";
			std::cin >> _rope_height;
			c.RopeMS()->Move("Z", _rope_height, 4.0);
		}
		else if (input == "calibrate") {
			c.RopeMS()->CalibratePresetValue();
			std::cout << "Reference position has been updated" << std::endl;
		}
		else if (input == "stop") {
			c.Coarse()->HaltX();
			c.Coarse()->HaltY();
			c.Fine()->HaltX();
			c.Fine()->HaltY();
			c.RopeMS()->Halt();
		}
		else if (input == "quit") {
			_general_stop_signal = true;
			_cv_avoidance_done.notify_all();
			_cv_collisiondetection_done.notify_all();
			break;
		}
		else if (input == "drop") {
			_crane.RopeMS()->ToGround(4.0);
		}
		else if (_general_stop_signal == true) {
			_cv_avoidance_done.notify_all();
			_cv_collisiondetection_done.notify_all();
			break;
		}
	}
}

/// <summary>
/// 
/// </summary>
void CraneSystemController::RetrieveStereoObstacles() {
	_correspondence.SetSamples(_left_detector.GetObstacles(), _right_detector.GetObstacles());
	_correspondence.Match();
	_obstacles = _correspondence.GetStereoObstacles();
}

/// <summary>
/// 
/// </summary>
void CraneSystemController::AvoidObstacles() {
	int x_remaining_dist = 0;
	int y_remaining_dist = 0;

	if(_x_distance > 0)
		x_remaining_dist = _x_distance - _crane.Coarse()->GetXCntValue();
	else if (_x_distance < 0)
		x_remaining_dist = _x_distance + _crane.Coarse()->GetXCntValue();

	if(_y_distance > 0)
		y_remaining_dist = _y_distance - _crane.Coarse()->GetYCntValue();
	else if (_y_distance < 0)
		y_remaining_dist = _y_distance + _crane.Coarse()->GetYCntValue();

	
	_rope_meter = this->GetHeighestCollidedObstacleValue();
	std::cout << _rope_meter << std::endl;
	_crane.RopeMS()->ElevateTo(_rope_meter, 4.0);
	this->Overpass(x_remaining_dist, y_remaining_dist);
	_crane.RopeMS()->ToGround(4.0);
	_state = State::STOP;
	_cv_avoidance_done.notify_all();
}

double CraneSystemController::GetHeighestCollidedObstacleValue() {
	double height = 0;
	for (auto o = _obstacles.begin(); o != _obstacles.end(); o++) {
			if ((*o).GetHeight() > height)
				height = (*o).GetHeight();
	}
	return height;
}

double CraneSystemController::GetHeighestObstacleValue() {
	double height = 0;
	for (auto o = _obstacles.begin(); o != _obstacles.end(); o++) {
		if ((*o).GetHeight() > height)
			height = (*o).GetHeight();
	}
	std::cout << height << std::endl;
	return height;
}

 

void CraneSystemController::Overpass(int x_steps, int y_steps) {
	int prev_left = _left_ropearea_collide;
	int prev_right = _right_ropearea_collide;
	double prev_rope_meter = _rope_meter;
	if (_x_distance > 0)
		_crane.Coarse()->X(0.3);
	else if (_x_distance < 0)
		_crane.Coarse()->X(-0.3);

	if (_y_distance > 0)
		_crane.Coarse()->Y(0.3);
	else if (_y_distance < 0)
		_crane.Coarse()->Y(-0.3);

	std::cout << "Started" << std::endl;
	if ((_x_distance != 0) || (_y_distance != 0)) {
		while (true) {
			if ((_left_ropearea_collide == 0) && (_right_ropearea_collide == 0)) {
				_crane.Coarse()->Halt();
				break;
			}

			if (_general_stop_signal)
				break;

			if((_left_ropearea_collide != prev_left) || (_right_ropearea_collide != prev_right) ){
				std::cout << "Left: " << _left_ropearea_collide << std::endl;
				std::cout << "Right: " << _right_ropearea_collide << std::endl;
				_crane.Coarse()->Halt();
				this->RetrieveStereoObstacles();
				_rope_meter = this->GetHeighestCollidedObstacleValue();
				if(_rope_meter > prev_rope_meter) {
					_crane.RopeMS()->ElevateTo(_rope_meter, 4.0);
					prev_rope_meter = _rope_meter;
				}
				if (_x_distance > 0)
					_crane.Coarse()->X(0.3);
				else if (_x_distance < 0)
					_crane.Coarse()->X(-0.3);

				if (_y_distance > 0)
					_crane.Coarse()->Y(0.3);
				else if (_y_distance < 0)
					_crane.Coarse()->Y(-0.3);
			}

			prev_left = _left_ropearea_collide;
			prev_right = _right_ropearea_collide;
		}
	}
	std::cout << "End" << std::endl;
}