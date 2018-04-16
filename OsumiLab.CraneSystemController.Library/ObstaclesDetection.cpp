#include "stdafx.h"
#include "ObstaclesDetection.h"

/// <summary>
/// 
/// </summary>
ObstaclesDetection::ObstaclesDetection() {
}

/// <summary>
/// 
/// </summary>
ObstaclesDetection::~ObstaclesDetection() {
}

/// <summary>
/// obstacles_ Getter
/// </summary>
/// <returns>Obstacle object</returns>
std::vector<Obstacle> ObstaclesDetection::GetObstacles() const {
	return this->obstacles_;
}

/// <summary>
/// Method to run the detection process
/// </summary>
/// <returns>Amount of obstacles detected</returns>
size_t ObstaclesDetection::Detect() {
	return this->obstacles_.size();
}
