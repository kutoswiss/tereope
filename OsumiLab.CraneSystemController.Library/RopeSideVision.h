#pragma once
#define _USE_MATH_DEFINES

#include "opencv2\opencv.hpp"
#include "CameraHelper.h"
#include "DrawRopeInfos.h"
#include "DrawHoughLines.h"
#include <math.h>
#include <memory>

class RopeSideVision
{
public:
	// Ctor/Dtor
	RopeSideVision();
	RopeSideVision(cv::Mat &input);
	~RopeSideVision();

	// Getters / Setters
	void SetFrame(cv::Mat &m);
	cv::Mat GetFrame();
	cv::Mat GetDecoratedFrame();
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
	cv::Mat _decorated_frame;
	cv::Mat _canny;
	cv::Mat _binary;
	std::unique_ptr<DrawTool> _draw_tool;
	cv::Mat _kernel;
};

