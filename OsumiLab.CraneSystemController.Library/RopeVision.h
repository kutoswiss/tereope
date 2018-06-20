#pragma once
#include "opencv2\opencv.hpp"
#include "CameraHelper.h"

class RopeVision
{
public:
	RopeVision();
	RopeVision(cv::Mat &input);
	~RopeVision();

	// Getters / Setters
	void SetFrame(cv::Mat &m);
	cv::Mat GetFrame();
	cv::Mat GetFrameWithLines();
	double GetAngle();

	// Public method
	void Compute();

private:
	// Private methods
	void ApplyHoughTransform();
	void CalculateAngle();

	// Private members
	double _angle;
	std::vector<cv::Vec4i> _lines;
	cv::Mat _frame;
	cv::Mat _frame_w_lines;
};

