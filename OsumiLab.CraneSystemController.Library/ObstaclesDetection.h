#pragma once
#include "opencv2\opencv.hpp"
#include "Obstacle.h"
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>

using namespace AVT::VmbAPI;

class ObstaclesDetection
{
public:
	/// <summary>
	/// Ctor/Dtor
	/// </summary>
	ObstaclesDetection();
	ObstaclesDetection(cv::Mat &input);
	~ObstaclesDetection();

	/// <summary>
	/// Private constants
	/// </summary>
	static const uint kDefaultMinContourArea = 125;
	static const uint kObstaclesPolygonTickness = 2;
	static const uint kBinaryThresholdValue = 25;
	const cv::Scalar kObstaclesPolygonColor = cv::Scalar(0, 255, 0);

	/// <summary>
	/// Properties
	/// </summary>
	/// <returns></returns>
	std::vector<Obstacle> GetObstacles() const;
	cv::Mat GetCannyFrame() const;
	cv::Mat GetFrameWithRectangles(bool info = false);
	void SetRawFrame(cv::Mat &frame);

	/// <summary>
	/// Public methods
	/// </summary>
	/// <returns></returns>
	size_t Detect();

private:
	/// <summary>
	/// Private methods
	/// </summary>
	std::vector<std::vector<cv::Point>>FindContoursOnFrame(
		uint area_threshold = kDefaultMinContourArea,
		uint contour_area = kDefaultMinContourArea);
	std::vector<cv::RotatedRect> CalcRotatedRects(
		std::vector<std::vector<cv::Point>> contours);
	void DrawObstaclesOnFrame(std::vector<Obstacle> obstacles, cv::Mat &frame);
	void DrawInfo(std::vector<Obstacle> obstacles, cv::Mat &frame);
	std::vector<Obstacle> RectsToObstacles(std::vector<cv::RotatedRect> rects);

	/// <summary>
	/// Private members
	/// </summary>
	std::vector<Obstacle> _obstacles;
	cv::Mat _raw_frame;
	cv::Mat _raw_frame_w_rects;
	cv::Mat _canny_frame;
	cv::Mat _binary_frame;
};