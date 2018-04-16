#pragma once
#include "opencv2\opencv.hpp"
#include "Obstacle.h"
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>

using namespace AVT::VmbAPI;

class ObstaclesDetection
{
public:
	/// <summary>
	/// Ctor/Dtor
	/// </summary>
	ObstaclesDetection();
	~ObstaclesDetection();

	/// <summary>
	/// Properties
	/// </summary>
	/// <returns></returns>
	std::vector<Obstacle> GetObstacles() const;

	/// <summary>
	/// Public methods
	/// </summary>
	/// <returns></returns>
	size_t Detect();

private:
	/// <summary>
	/// Private members
	/// </summary>
	std::vector<Obstacle> obstacles_;
};