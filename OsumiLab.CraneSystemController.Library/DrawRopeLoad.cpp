#include "stdafx.h"
#include "DrawRopeLoad.h"


DrawRopeLoad::DrawRopeLoad(cv::Mat &frame, Obstacle load) {
	_load = load;
	this->SetFrame(frame);
}


DrawRopeLoad::~DrawRopeLoad() {
}

/// <summary>
/// 
/// </summary>
void DrawRopeLoad::Draw() {
	// Get points from rotated rectangles
	std::vector<std::vector<cv::Point>> points;
	std::vector<std::vector<cv::Point>> collision_area_points;

	points.push_back(_load.ToPoints());
	collision_area_points.push_back(_load.GetCollisionArea().ToPoints());

	// Draw polylines from points
	cv::polylines(this->_frame, points,
		true, kObstaclesPolygonColor, 2, CV_AA);

	cv::polylines(this->_frame, collision_area_points,
		true, kCollisionAreaPolygonColor, 1, CV_AA);
}
