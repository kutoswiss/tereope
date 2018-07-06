// OsumiLab.CraneSystemController.GUI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Demo.h"

//#include "ObstaclesDetection.h"
//#include "ObstaclesCorrespondence.h"
//#include "Crane.h"
//#include "RopeSwingRegulator.h"
//
//#include <vector>
//#include <iostream>
//#include <thread>
//#include <mutex>Å@
//#include <condition_variable>Å@
//#include <chrono>    
//#include <string>

void CollisionDetectionTask(Crane &c);
void RopeSwingingRegulationTask(Crane &c);
void InputReaderTask(Crane &c);
bool stop = false;

int main() {
	Crane c;
	std::thread input_reader_task(InputReaderTask, std::ref(c));
	//std::thread collision_detection_task(CollisionDetectionTask, std::ref(c));
	std::thread swing_regulator_task(RopeSwingingRegulationTask, std::ref(c));

	input_reader_task.join();
	swing_regulator_task.join();
	//collision_detection_task.join();

    return 0;
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void CollisionDetectionTask(Crane &c) {

}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void RopeSwingingRegulationTask(Crane &c) {
	RopeSwingRegulator regulator;
	regulator.Regulate(std::ref(c), &stop);
}

/// <summary>
/// 
/// </summary>
/// <param name="c"></param>
void InputReaderTask(Crane &c) {
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
			stop = true;
			break;
		}
	}
}

