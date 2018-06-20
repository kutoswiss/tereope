#pragma once
#include "opencv2\opencv.hpp"
#include "CameraHelper.h"

class RopeSideVision
{
public:
	RopeSideVision();
	RopeSideVision(cv::Mat &input);
	~RopeSideVision();

	// Getters / Setters
	void SetFrame(cv::Mat &m);
	cv::Mat GetFrame();
	cv::Mat GetFrameWithLines();
	double GetAngle();

	// Public method
	void Compute();

private:
	// Private methods
	void FindLines();
	void CalculateAngle();

	// Private members
	double _angle;
	std::vector<cv::Vec4i> _lines;
	cv::Mat _frame;
	cv::Mat _frame_w_lines;
};

