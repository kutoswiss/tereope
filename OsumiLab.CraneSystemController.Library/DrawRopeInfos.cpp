#include "stdafx.h"
#include "DrawRopeInfos.h"

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
DrawRopeInfos::DrawRopeInfos(cv::Mat &frame, int lines, double angle) {
	_lines = lines;
	_angle = angle;
	this->SetFrame(frame);
}

/// <summary>
/// 
/// </summary>
DrawRopeInfos::~DrawRopeInfos() {
}

/// <summary>
/// 
/// </summary>
void DrawRopeInfos::Draw() {
	cv::putText(_frame, this->GetLinesText() , cv::Point(10, 10),
		this->kTextFont, this->kTextSize, this->kTextColor);

	cv::putText(_frame, this->GetAngleText(), cv::Point(10, 20),
		this->kTextFont, this->kTextSize, this->kTextColor);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::string DrawRopeInfos::GetLinesText() {
	_lines_ss.clear();
	_lines_ss << "Hough lines: " << _lines;
	return _lines_ss.str();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::string DrawRopeInfos::GetAngleText() {
	_angle_ss.clear();
	_angle_ss << "Angle: " << std::fixed << std::setprecision(4) << _angle;
	return _angle_ss.str();
}