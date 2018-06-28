#pragma once
#include "DrawTool.h"

class DrawHoughLines : public DrawTool
{
public:
	// Constants
	const cv::Scalar kLineColor = cv::Scalar(255, 255, 0);
	const double kLineTickness = 1;

	// Ctor/Dtor
	DrawHoughLines(cv::Mat &frame, std::vector<cv::Vec4i> lines);
	~DrawHoughLines();

private:
	// Private methods
	void Draw();

	// Private members
	std::vector<cv::Vec4i> _lines;
};

