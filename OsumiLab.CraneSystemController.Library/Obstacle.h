#pragma once
#include "opencv2\opencv.hpp"

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
	uint GetArea() const;

    /// <summary>
    /// Public methods
    /// </summary>
    std::string ToString();
    std::vector<cv::Point> ToPoints();

private:
    /// <summary>
    /// Private members
    /// </summary>
    uint _height;
    cv::RotatedRect _rotated_rect;
};

