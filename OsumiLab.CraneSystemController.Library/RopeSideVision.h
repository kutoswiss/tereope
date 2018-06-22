#pragma once
#define _USE_MATH_DEFINES

#include "opencv2\opencv.hpp"
#include "CameraHelper.h"
#include <math.h>

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
	cv::Mat GetCannyFrame();
	cv::Mat GetBinaryFrame();
	double GetAngle();

	// Public method
	void Compute();

private:
	// Private methods
	void FindLines();
	void LinearRegression(std::vector<cv::Point> points, double *a, double *b);
	void CalculateAngle();

	// Private members
	double _angle;
	std::vector<cv::Vec4i> _lines;
	cv::Mat _frame;
	cv::Mat _frame_w_lines;
	cv::Mat _canny;
	cv::Mat _binary;
};

