#pragma once
#include "StereoObstacle.h"
#include <vector>

class ObstaclesCorrespondence
{
public:
	// Ctor/Dtor
	ObstaclesCorrespondence();
	ObstaclesCorrespondence(
		std::vector<Obstacle> left_samples,
		std::vector<Obstacle> right_samples);
	~ObstaclesCorrespondence();

	// Public methods
	void SetSamples(std::vector<Obstacle> left_samples, std::vector<Obstacle> right_samples);
	std::vector<StereoObstacle> Match(void);
	void PrintMatchedObstacles(void);

	std::vector<StereoObstacle> GetStereoObstacles();

private:
	// Private methods
	bool IsMatched(Obstacle l, Obstacle r);

	// Private members
	std::vector<Obstacle> _left_samples;
	std::vector<Obstacle> _right_samples;
	std::vector<StereoObstacle> _matched_obstacles;
};

