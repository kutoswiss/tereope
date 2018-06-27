#include "stdafx.h"
#include "DrawObstaclesCorners.h"


DrawObstaclesCorners::DrawObstaclesCorners(cv::Mat &frame, std::vector<Obstacle> obstacles) {
	_obstacles = obstacles;
	this->SetFrame(frame);
}

DrawObstaclesCorners::~DrawObstaclesCorners() {
}

/// <summary>
/// 
/// </summary>
void DrawObstaclesCorners::Draw() {
	this->DrawCorners(_obstacles, 2);
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacles"></param>
void DrawObstaclesCorners::DrawCorners(std::vector<Obstacle> obstacles, uint tickness) {
	for (auto it = obstacles.begin(); it != obstacles.end(); it++) {
		for (int i = 0; i < 4; i++) {
			cv::circle(this->_frame, (*it).ToPoints()[i], 2, this->kObstacleCornerColor, 2);
		}
	}
}
