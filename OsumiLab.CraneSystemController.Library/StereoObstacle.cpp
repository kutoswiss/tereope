#include "stdafx.h"
#include "StereoObstacle.h"

/// <summary>
/// 
/// </summary>
StereoObstacle::StereoObstacle() {

}

/// <summary>
/// 
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
StereoObstacle::StereoObstacle(Obstacle left, Obstacle right) {
	this->_left = left;
	this->_right = right;
	this->EvaluateHeight();
}

/// <summary>
/// 
/// </summary>
StereoObstacle::~StereoObstacle() {
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
Obstacle StereoObstacle::GetLeftObstacle() const {
	return this->_left;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
Obstacle StereoObstacle::GetRightObstacle() const {
	return this->_right;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
double StereoObstacle::GetHeight() const {
	return this->_height;
}


/// <summary>
/// 
/// </summary>
/// <returns></returns>
double StereoObstacle::EvaluateHeight() {
	const double kFocalLenght = 0.00651;
	const double kPixelSize = 0.0000074;
	const double kCaptureDistance = 0.08;
	const double kCameraHeight = 1.60; 
	double x_delta = this->_left.GetCenter().x - this->_right.GetCenter().x;
	double d = x_delta * kPixelSize;
	double z = std::abs(kFocalLenght * kCaptureDistance / d);
	this->_height = kCameraHeight - z;
	return this->_height;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool StereoObstacle::IsCollided() {
	return _left.collide || _right.collide;
}
