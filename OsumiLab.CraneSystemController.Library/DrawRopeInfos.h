#pragma once
#include "DrawTool.h"

class DrawRopeInfos : public DrawTool
{
public:
	// Constants
	const cv::Scalar kTextColor = cv::Scalar(255, 255, 255);
	const double kTextSize = 0.6;
	const int kTextFont = cv::FONT_HERSHEY_PLAIN;

	// Ctor/Dtor
	DrawRopeInfos(cv::Mat &frame, int lines, double angle);
	~DrawRopeInfos();

private:
	// Private methods
	void Draw();
	std::string GetLinesText();
	std::string GetAngleText();

	// Private members
	int _lines;
	double _angle;
	std::stringstream _lines_ss, _angle_ss;
};

