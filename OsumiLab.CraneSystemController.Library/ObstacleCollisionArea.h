#pragma once
#include "opencv2\opencv.hpp"

class ObstacleCollisionArea
{
public:
	ObstacleCollisionArea();
	ObstacleCollisionArea(cv::RotatedRect collision_area);
	~ObstacleCollisionArea();

	// Getter/Setters
	cv::RotatedRect GetRect() const;
	void SetArea(cv::Point center, double angle, int width, int height);

	// Public methods
	std::vector<cv::Point> ToPoints();
	std::vector<std::tuple<cv::Point, cv::Point>> ToSegments();
	bool CollideWith(ObstacleCollisionArea area);

private: 
	// Private metods
	bool SegmentsIntersects(cv::Point p, cv::Point pr, cv::Point q, cv::Point qs);
	
	// Private members
	cv::RotatedRect _collision_area;
};

