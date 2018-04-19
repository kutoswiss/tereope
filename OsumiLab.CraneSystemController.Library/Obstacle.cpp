#include "stdafx.h"
#include "Obstacle.h"

/// <summary>
/// 
/// </summary>
Obstacle::Obstacle(cv::RotatedRect rotated_rect) {
	this->rotated_rect_ = rotated_rect;
}

/// <summary>
/// 
/// </summary>
Obstacle::~Obstacle() {
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
uint Obstacle::GetAngle() const {
	return this->rotated_rect_.angle;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Point2f Obstacle::GetCenter() const{
	return this->rotated_rect_.center;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::RotatedRect Obstacle::GetRect() const {
	return this->rotated_rect_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::string Obstacle::ToString() {
	std::stringstream ss;
	ss << "Origin: " << (int)this->rotated_rect_.center.x
		<< "," << (int)this->rotated_rect_.center.y << '\r\n'
		<< "Angle: " << std::setprecision(2) << this->rotated_rect_.angle << " deg";
	return ss.str();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::vector<cv::Point> Obstacle::ToPoints() {
	const uint kVerticesSize = 4;
	cv::Point2f vertices[kVerticesSize];
	std::vector<cv::Point> pts;

	this->rotated_rect_.points(vertices);
	for (int i = 0; i < kVerticesSize; i++)
		pts.push_back(vertices[i]);

	return pts;
}
