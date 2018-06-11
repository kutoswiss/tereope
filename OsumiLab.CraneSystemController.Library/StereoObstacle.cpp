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
	const double kCaptureDistance = 0.10;
	const double kCameraHeight = 1.62; 
	double d = (this->_left.GetCenter().x - this->_right.GetCenter().x) * kPixelSize;
	double z = std::abs(static_cast<double>((kFocalLenght * kCaptureDistance) / d));
	this->_height = kCameraHeight - z;
	return this->_height;
}
