#include "stdafx.h"
#include "CraneSystemController.h"

/// <summary>
/// 
/// </summary>
CraneSystemController::CraneSystemController() {
	_general_stop_signal = false;
	_left_detector.SetRopeLoadAreaOrigin(cv::Point(350, 325));
	_state = State::MOVING;
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
	_cmd_task = std::make_unique<std::thread>(
		&CraneSystemController::CommandTask,
		this, 
		std::ref(_crane));

	_leftcam_collision_detection_task = std::make_unique<std::thread>(
		&CraneSystemController::LeftCamCollisionDetectionTask,
		this,
		std::ref(_crane));

	_rightcam_collision_detection_task = std::make_unique<std::thread>(
		&CraneSystemController::RightCamCollisionDetectionTask,
		this,
		std::ref(_crane));

	_obstacle_avoidance_task = std::make_unique<std::thread>(
		&CraneSystemController::ObstacleAvoidanceTask,
		this,
		std::ref(_crane));

	//_rope_regulation_task = std::make_unique<std::thread>(
	//	&CraneSystemController::RopeSwingingRegulationTask,
	//	this,
	//	std::ref(c));

	//_yrope_regulation_task = std::make_unique<std::thread>(
	//	&CraneSystemController::YRopeSwingingRegulationTask,
	//	this,
	//	std::ref(c));

	//_xrope_regulation_task = std::make_unique<std::thread>(
	//	&CraneSystemController::XRopeSwingingRegulationTask, 
	//	this, 
	//	std::ref(c));

	_cmd_task->join();
	//_rope_regulation_task->join();
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
	while (_general_stop_signal == false) {
		_leftcam = c.LeftSceneCamera().GetMat(CV_8UC1);
		_left_detector.SetRawFrame(_leftcam);
		_left_detector.Detect();

		if ((_state == State::MOVING) && _left_detector.RopeLoadCollidesWithObstacles()) {
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
	while (_general_stop_signal == false) {
		_rightcam = c.RightSceneCamera().GetMat(CV_8UC1);
		_right_detector.SetRawFrame(_rightcam);
		_right_detector.Detect();

		if ((_state == State::MOVING) && _right_detector.RopeLoadCollidesWithObstacles()) {
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
void CraneSystemController::CollisionDetectionTask(Crane &c) {
	ObstaclesDetection left_detector, right_detector;
	ObstaclesCorrespondence correspondence;

	left_detector.SetRopeLoadAreaOrigin(cv::Point(350, 325));
	short i = 0;

	while (true) {
		_leftcam = c.LeftSceneCamera().GetMat(CV_8UC1);
		_rightcam = c.RightSceneCamera().GetMat(CV_8UC1);

		left_detector.SetRawFrame(_leftcam);
		right_detector.SetRawFrame(_rightcam);

		left_detector.Detect();
		right_detector.Detect();

		if (right_detector.RopeLoadCollidesWithObstacles() || left_detector.RopeLoadCollidesWithObstacles()) {
			c.Coarse()->Halt();
		}

		cv::imshow("Left scene camera", left_detector.GetFrameWithRectangles());
		cv::imshow("Right scene camera", right_detector.GetFrameWithRectangles());

		if ((cv::waitKey(50) >= 0) || (_general_stop_signal)) {
			break;
		}
	}

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
		_state = State::AVOIDING_OBSTACLES;
		if (_general_stop_signal == true)
			break;

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
			c.Coarse()->X(_x_distance, 0.3);
		}
		else if (input == "y") {
			std::cout << "> Enter Y value: ";
			std::cin >> _y_distance;
			c.Coarse()->Y(_y_distance, 0.3);
		}
		else if (input == "xt") {
			std::cout << "> Enter X value: ";
			std::cin >> _x_distance;
			c.Coarse()->XThread(_x_distance, 0.3);
			_state = State::MOVING;
		}
		else if (input == "yt") {
			std::cout << "> Enter Y value: ";
			std::cin >> _y_distance;
			c.Coarse()->YThread(_y_distance, 0.3);
		}
		else if (input == "newxy") {
			std::cout << "> Enter X value: ";
			std::cin >> _x_distance;
			std::cout << "> Enter Y value: ";
			std::cin >> _y_distance;
			c.Coarse()->XThread(_x_distance, 0.3);
			c.Coarse()->YThread(_y_distance, 0.3);
		}
		else if (input == "xy") {
			std::cout << "> Enter X value: ";
			std::cin >> _x_distance;
			std::cout << "> Enter Y value: ";
			std::cin >> _y_distance;
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
	double height = 0;
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

	for (auto o = _obstacles.begin(); o != _obstacles.end(); o++) {
		if ((*o).GetHeight() > height)
			height = (*o).GetHeight();
	}

	std::cout << height << std::endl;

	//_crane.RopeMS()->ElevateTo(height, 4.0);
	this->Overpass(x_remaining_dist, y_remaining_dist);
	//_crane.RopeMS()->ToGround(4.0);
	//_crane.RopeMS()->CalibratePresetValue();
	_state = State::MOVING;
	_cv_avoidance_done.notify_all();
}

void CraneSystemController::Overpass(int x_steps, int y_steps) {
	/*if(_x_distance > 0)
		_crane.Coarse()->X(x_steps, 0.3);
	else if (_x_distance < 0)
		_crane.Coarse()->X(x_steps * -1, 0.3);

	if (_y_distance > 0)
		_crane.Coarse()->Y(y_steps, 0.3);
	else if (_y_distance < 0)
		_crane.Coarse()->Y(y_steps * -1, 0.3);*/
	_x_distance = 1;
	std::cout << "Started" << std::endl;
	if ((_x_distance != 0) || (_y_distance != 0)) {
		while (true)
		{
			if (_left_detector.RopeAreaCollidesWithObstacles() == false)
				break;

			if (_right_detector.RopeAreaCollidesWithObstacles() == false)
				break;

			if (_general_stop_signal)
				break;
		}
	}
	std::cout << "End" << std::endl;

	//_crane.Coarse()->Halt();

	//if ((_x_distance != 0) || (_y_distance != 0)) {
	//	while (true) {
	//		if (!_left_detector.RopeLoadCollidesWithObstacles() && !_right_detector.RopeLoadCollidesWithObstacles())
	//			break;

	//		if ((cv::waitKey(50) >= 0) || (_general_stop_signal)) {
	//			break;
	//		}
	//	}

	//	_crane.Coarse()->Halt();
	//	cv::destroyAllWindows();
	//}
	
}