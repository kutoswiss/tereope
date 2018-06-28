#include "stdafx.h"
#include "DrawRopeLoadArea.h"

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
/// <param name="rope_load_area"></param>
DrawRopeLoadArea::DrawRopeLoadArea(cv::Mat &frame, RopeLoadArea rope_load_area) {
	_rope_load_area = rope_load_area;
	this->SetFrame(frame);
}

/// <summary>
/// 
/// </summary>
DrawRopeLoadArea::~DrawRopeLoadArea() {
}

/// <summary>
/// 
/// </summary>
void DrawRopeLoadArea::Draw() {
	// Get points from rotated rectangles
	std::vector<std::vector<cv::Point>> points;
	points.push_back(_rope_load_area.ToPoints());

	// Draw polylines from points
	cv::polylines(this->_frame, points,
		true, kObstaclesPolygonColor, 1, CV_AA);
}
