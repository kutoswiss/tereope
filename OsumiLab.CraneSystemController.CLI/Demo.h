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

class Demo
{
public:
	// Stereo correspondance demos
	static void SingleCameraStereoCorrespondance(int steps);

	// Stereo vision demos
	static void DoubleCamerasStereoVision();

	// Obstacles detection
	static void SceneCameraThread(CraneSceneCamera &camera, ObstaclesDetection &obstacle_detection);
	static void SingleCameraObstaclesDetection();

	// Rope demos
	static void RopeSwingRegulation();

	// Cameras
	static void PrintCamerasRetrievedTime();
};

