#include "stdafx.h"
#include "ObstaclesDraw.h"

ObstaclesDraw::ObstaclesDraw() {
	this->_textinfos = true;
	this->_corners = true;
	this->_origins = true;
}

ObstaclesDraw::ObstaclesDraw(cv::Mat &frame) : ObstaclesDraw() {
	this->_frame = frame;
}

ObstaclesDraw::~ObstaclesDraw() {
}

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
void ObstaclesDraw::SetFrame(cv::Mat &frame) {
	this->_frame = frame;
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacles"></param>
/// <param name="tickness"></param>
void ObstaclesDraw::Draw(std::vector<Obstacle> obstacles, uint tickness) {
	this->DrawObstacles(obstacles, tickness);
	this->DrawAdditionnalInfos(obstacles);
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacles"></param>
void ObstaclesDraw::DrawObstacles(std::vector<Obstacle> obstacles, uint tickness) {
	// Get points from rotated rectangles
	std::vector<std::vector<cv::Point>> points;
	for (auto it = obstacles.begin(); it != obstacles.end(); it++)
		points.push_back((*it).ToPoints());

	// Draw polylines from points
	cv::polylines(this->_frame, points,
		true, kObstaclesPolygonColor, tickness, CV_AA);
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacle"></param>
void ObstaclesDraw::DrawTextInfos(Obstacle obstacle) {
	std::stringstream ss;
	cv::Point origin;
	cv::RotatedRect rect;

	rect = obstacle.GetRect();
	origin = cv::Point(rect.center.x - 10 - rect.size.height / 2,
		rect.center.y - 10 - rect.size.width / 2);

	cv::putText(this->_frame, obstacle.ToString(), origin,
		cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(0, 255, 0));
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacle"></param>
/// <param name="frame"></param>
void ObstaclesDraw::DrawOrigin(Obstacle obstacle) {
	cv::circle(this->_frame, obstacle.GetCenter(), 2, this->kObstaclesOriginColor, 2);
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacle"></param>
void ObstaclesDraw::DrawCorners(Obstacle obstacle) {
	cv::circle(this->_frame, obstacle.ToPoints()[0], 2, this->kObstaclesCornerColor, 2);
	cv::circle(this->_frame, obstacle.ToPoints()[1], 2, this->kObstaclesCornerColor, 2);
	cv::circle(this->_frame, obstacle.ToPoints()[2], 2, this->kObstaclesCornerColor, 2);
	cv::circle(this->_frame, obstacle.ToPoints()[3], 2, this->kObstaclesCornerColor, 2);
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacles"></param>
void ObstaclesDraw::DrawAdditionnalInfos(std::vector<Obstacle> obstacles) {
	for (auto it = obstacles.begin(); it != obstacles.end(); it++) {
		if (this->_textinfos)
			this->DrawTextInfos((*it));

		if (this->_origins)
			this->DrawOrigin((*it));

		if (this->_corners)
			this->DrawCorners((*it));
	}
}
