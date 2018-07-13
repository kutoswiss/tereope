#pragma once
#include "ObstacleCollisionArea.h"

class RopeLoadArea
{
public:
	// Ctor/Dtor
	RopeLoadArea();
	RopeLoadArea(double width, double height, cv::Point center);
	~RopeLoadArea();

	// Getter/Setters
	double X() const;
	double Y() const;
	double Width() const;
	double Height() const;
	cv::Point Center() const;
	void Set(double width, double height, cv::Point center);
	ObstacleCollisionArea GetCollisionArea();

	// Public methods
	std::vector<cv::Point> ToPoints();
	
private:
	// Private members
	cv::RotatedRect _rect;
};

