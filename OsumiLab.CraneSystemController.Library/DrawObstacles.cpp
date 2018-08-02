#include "stdafx.h"
#include "DrawObstacles.h"

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
/// <param name="obstacles"></param>
DrawObstacles::DrawObstacles(cv::Mat &frame, std::vector<Obstacle> obstacles){
	_obstacles = obstacles;
	this->SetFrame(frame);
}

/// <summary>
/// 
/// </summary>
DrawObstacles::~DrawObstacles() {
}

/// <summary>
/// 
/// </summary>
void DrawObstacles::Draw() {
	this->DrawObstaclesOnFrame(_obstacles, 2);
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacles"></param>
void DrawObstacles::DrawObstaclesOnFrame(std::vector<Obstacle> obstacles, uint tickness) {
	// Get points from rotated rectangles
	std::vector<std::vector<cv::Point>> points;
	std::vector<std::vector<cv::Point>> collided_area_points;
	std::vector<std::vector<cv::Point>> uncollided_area_points;


	for (auto it = obstacles.begin(); it != obstacles.end(); it++)
		points.push_back((*it).ToPoints());

	for (auto it = obstacles.begin(); it != obstacles.end(); it++) {
		if ((*it).collide)
			collided_area_points.push_back((*it).GetCollisionArea().ToPoints());
		else
			uncollided_area_points.push_back((*it).GetCollisionArea().ToPoints());
	}
		


	// Draw polylines from points
	cv::polylines(this->_frame, points,
		true, kObstaclesPolygonColor, tickness, CV_AA);

	cv::polylines(this->_frame, collided_area_points,
		true, kCollidedAreaPolygonColor, 1, CV_AA);

	cv::polylines(this->_frame, uncollided_area_points,
		true, kUncollidedAreaPolygonColor, 1, CV_AA);
}
