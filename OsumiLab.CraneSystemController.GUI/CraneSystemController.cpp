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
		&CraneSystemController::CommandTask, this, std::ref(c));

	_rope_regulation_task = std::make_unique<std::thread>(
		&CraneSystemController::RopeSwingingRegulationTask, this, std::ref(c));
	
	_cmd_task->join();
	_rope_regulation_task->join();
}


/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CraneSystemController::CollisionDetectionTask(Crane &c) {

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
void CraneSystemController::CommandTask(Crane &c) {
	int x_val = 0;
	int y_val = 0;
	std::string input;

	while (true) {
		std::cout << "> ";
		std::cin >> input;
		if (input == "x") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			c.CoarseAxis().MoveThread(Axis::X, x_val, 0.5);
		}
		else if (input == "y") {
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.CoarseAxis().MoveThread(Axis::Y, y_val, 0.5);
		}
		else if (input == "xy") {
			std::cout << "> Enter X value: ";
			std::cin >> x_val;
			std::cout << "> Enter Y value: ";
			std::cin >> y_val;
			c.CoarseAxis().MoveThread(Axis::X, x_val, 0.5);
			c.CoarseAxis().MoveThread(Axis::Y, y_val, 0.5);
		}
		else if (input == "stop") {
			c.CoarseAxis().Stop(Axis::X);
			c.CoarseAxis().Stop(Axis::Y);
			std::cout << c.CoarseAxis().GetCntValue(Axis::X) << std::endl;

		}
		else if (input == "quit") {
			_general_stop_signal = true;
			break;
		}
	}
}

