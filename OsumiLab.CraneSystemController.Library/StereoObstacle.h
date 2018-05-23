#pragma once
#include "Obstacle.h"

class StereoObstacle
{
public:
	// Ctor/Dtor
	StereoObstacle();
	StereoObstacle(Obstacle left, Obstacle right);
	~StereoObstacle();
	
	// Public methods
	Obstacle GetLeftObstacle() const;
	Obstacle GetRightObstacle() const;
	int GetHeight() const;

private:
	// Private methods
	int EvaluateHeight();

	// Private members
	Obstacle _left;
	Obstacle _right;
};

