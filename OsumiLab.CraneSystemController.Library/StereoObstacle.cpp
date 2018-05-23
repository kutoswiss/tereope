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
