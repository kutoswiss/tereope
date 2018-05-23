#include "stdafx.h"
#include "Obstacle.h"

/// <summary>
/// 
/// </summary>
Obstacle::Obstacle() {
}

/// <summary>
/// 
/// </summary>
Obstacle::Obstacle(cv::RotatedRect rotated_rect) {
    this->_rotated_rect = rotated_rect;
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
    return this->_rotated_rect.angle;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Point2f Obstacle::GetCenter() const{
    return this->_rotated_rect.center;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::RotatedRect Obstacle::GetRect() const {
    return this->_rotated_rect;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
uint Obstacle::GetArea() const {
	return this->_rotated_rect.size.area();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::string Obstacle::ToString() {
    std::stringstream ss;
    ss << "Origin: " << (int)this->_rotated_rect.center.x
        << "," << (int)this->_rotated_rect.center.y
        << " / Angle: " << std::setprecision(2) << this->_rotated_rect.angle << " deg";
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

    this->_rotated_rect.points(vertices);
    for (int i = 0; i < kVerticesSize; i++)
        pts.push_back(vertices[i]);

    return pts;
}
