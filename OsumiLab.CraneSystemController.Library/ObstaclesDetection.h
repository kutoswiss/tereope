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
	ObstaclesDetection(cv::Mat &input);
	~ObstaclesDetection();

	/// <summary>
	/// Private constants
	/// </summary>
	static const unsigned int kDefaultMinContourArea = 125;
	static const unsigned int kObstaclesPolygonTickness = 2;
	static const unsigned int kBinaryThresholdValue = 25;
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
	std::vector<std::vector<cv::Point>> FindContours(
		unsigned int area_threshold = kDefaultMinContourArea,
		unsigned int contour_area = kDefaultMinContourArea);
	
	std::vector<cv::RotatedRect> CalcRotatedRects(
		std::vector<std::vector<cv::Point>> contours);

	void DrawRotatedRects(std::vector<cv::RotatedRect> rects, cv::Mat &frame);
	void DrawInfo(std::vector<cv::RotatedRect> rects, cv::Mat &frame);

	/// <summary>
	/// Private members
	/// </summary>
	std::vector<Obstacle> obstacles_;
	std::vector<cv::RotatedRect> rotated_rects_;
	cv::Mat raw_frame_;
	cv::Mat raw_frame_w_rects_;
	cv::Mat canny_frame_;
	cv::Mat binary_frame_;
};