#include "stdafx.h"
#include "DrawObstaclesInfos.h"


DrawObstaclesInfos::DrawObstaclesInfos(cv::Mat &frame, std::vector<Obstacle> obstacles) {
	_obstacles = obstacles;
	this->SetFrame(frame);
}

DrawObstaclesInfos::~DrawObstaclesInfos() {
}

/// <summary>
/// 
/// </summary>
void DrawObstaclesInfos::Draw() {
	this->DrawInfos(_obstacles);
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacles"></param>
void DrawObstaclesInfos::DrawInfos(std::vector<Obstacle> obstacles) {
	for (auto it = obstacles.begin(); it != obstacles.end(); it++) {
		std::stringstream ss;
		cv::Point origin;
		cv::RotatedRect rect;

		rect = (*it).GetRect();
		origin = cv::Point(rect.center.x - 10 - rect.size.height / 2,
			rect.center.y - 10 - rect.size.width / 2);

		cv::putText(this->_frame, (*it).ToString(), origin,
			cv::FONT_HERSHEY_PLAIN, 0.7, this->kTextColor);
	}
}
