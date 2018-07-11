#include "stdafx.h"
#include "CraneSystemController.h"

/// <summary>
/// 
/// </summary>
CraneSystemController::CraneSystemController() {
	_general_stop_signal = false;
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
	Crane c;

	_cmd_task = std::make_unique<std::thread>(
		&CraneSystemController::CommandTask,
		this, 
		std::ref(c));

	_rope_regulation_task = std::make_unique<std::thread>(
		&CraneSystemController::RopeSwingingRegulationTask, 
		this, 
		std::ref(c));

	_collision_detection_task = std::make_unique<std::thread>(
		&CraneSystemController::CollisionDetectionTask,
		this,
		std::ref(c));
	
	_cmd_task->join();
	_rope_regulation_task->join();
	_collision_detection_task->join();
}


/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::CollisionDetectionTask(Crane &c) {
	ObstaclesDetection left_detector, right_detector;
	ObstaclesCorrespondence correspondence;
	cv::Mat left, right;

	left_detector.SetRopeLoadAreaOrigin(cv::Point(350, 325));

	while(true) {
		left = c.LeftSceneCamera().GetMat(CV_8UC1);
		right = c.RightSceneCamera().GetMat(CV_8UC1);

		left_detector.SetRawFrame(left);
		right_detector.SetRawFrame(right);

		left_detector.Detect();
		right_detector.Detect();

		if ((left_detector.RopeLoadCollidesWithObstacles()) || (right_detector.RopeLoadCollidesWithObstacles()))
			c.Coarse()->Halt();

		cv::imshow("Left scene camera", left_detector.GetFrameWithRectangles());
		cv::imshow("Right scene camera", right_detector.GetFrameWithRectangles());

		if ((cv::waitKey(15) >= 0) || (_general_stop_signal)) break;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::RopeSwingingRegulationTask(Crane &c) {
	RopeSwingRegulator regulator;
	//regulator.Regulate(std::ref(c), &_general_stop_signal);
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::CommandTask(Crane &c) {
	double x_val = 0;
	double y_val = 0;
	double z_val = 0;
	std::string input;

	while (true) {
		std::cout << "> ";
		std::cin >> input;
		if (input == "x") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			c.Coarse()->X(x_val, 0.3);
			//c.CoarseAxis().MoveThread(Axis::X, x_val, 0.5);
		}
		else if (input == "y") {
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.Coarse()->Y(y_val, 0.3);
			//c.CoarseAxis().MoveThread(Axis::Y, y_val, 0.5);
		}
		else if (input == "xt") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			c.Coarse()->XThread(x_val, 0.3);
		}
		else if (input == "yt") {
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.Coarse()->YThread(y_val, 0.3);
		}
		else if (input == "newxy") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.Coarse()->XThread(x_val, 0.3);
			c.Coarse()->YThread(y_val, 0.3);
		}
		else if (input == "xy") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			/*		c.CoarseAxis().MoveThread(Axis::X, x_val, 0.5);
					c.CoarseAxis().MoveThread(Axis::Y, y_val, 0.5);*/
		}
		else if (input == "finex") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			c.Fine()->X(x_val);
		}
		else if (input == "finey") {
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.Fine()->Y(y_val);
		}
		else if (input == "rope") {
			/*std::cout << "> Enter Z value in meter: ";
			std::cin >> z_val;
			c.RopeMS()->Elevate(z_val, 2.0);*/
			c.RopeMS()->CalibratePresetValue();
			c.RopeMS()->ElevateTo(0.6, 4.0);
			c.RopeMS()->ToGround(4.0);
		}
		else if (input == "stop") {
			//c.CoarseAxis().Stop(Axis::X);
			//c.CoarseAxis().Stop(Axis::Y);
			//std::cout << c.CoarseAxis().GetCntValue(Axis::X) << std::endl;
			c.Coarse()->HaltX();
			c.Coarse()->HaltY();
			c.Fine()->HaltX();
			c.Fine()->HaltY();
		}
		else if (input == "quit") {
			_general_stop_signal = true;
			break;
		}
		else if (_general_stop_signal == true)
			break;
	}
}

