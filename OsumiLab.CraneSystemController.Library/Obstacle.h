#pragma once
#include "opencv2\opencv.hpp"
#include "ObstacleCollisionArea.h"

class Obstacle
{
public:
    /// <summary>
    /// Ctor/Dtor
    /// </summary>
	Obstacle();
    Obstacle(cv::RotatedRect rotated_rect);
    ~Obstacle();

    /// <summary>
    /// Properties
    /// </summary>
    /// <returns></returns>
    uint GetAngle() const;
    cv::Point2f GetCenter() const;
    cv::RotatedRect GetRect() const;
	ObstacleCollisionArea GetCollisionArea() const;
	uint GetArea() const;
	uint GetHeight() const;
	uint GetWidth() const;

    /// <summary>
    /// Public methods
    /// </summary>
    std::string ToString();
    std::vector<cv::Point> ToPoints();
	std::vector<std::tuple<cv::Point, cv::Point>> ToSegments();
	bool CollideWith(Obstacle obstacle);
	bool CollideWith(ObstacleCollisionArea area);

private:
    /// <summary>
    /// Private members
    /// </summary>
    uint _height;
    cv::RotatedRect _rotated_rect;
	ObstacleCollisionArea _collision_area;
};

