#include "stdafx.h"
#include "StereoObstacle.h"


StereoObstacle::StereoObstacle() {

}

StereoObstacle::StereoObstacle(Obstacle left, Obstacle right) {
	this->_left = left;
	this->_right = right;
}

StereoObstacle::~StereoObstacle()
{
}

Obstacle StereoObstacle::GetLeftObstacle() const {
	return this->_left;
}

Obstacle StereoObstacle::GetRightObstacle() const {
	return this->_right;
}

double StereoObstacle::EvaluateHeight() {
	const double kFocalLenght = 0.00651;
	const double kPixelSize = 0.0000074;
	const double kCaptureDistance = 0.10;
	double d = (this->_left.GetCenter().x - this->_right.GetCenter().x) * kPixelSize;
	return (kFocalLenght * kCaptureDistance) / d;
}
