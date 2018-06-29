#include "stdafx.h"
#include "RopeSideVision.h"

/// <summary>
/// 
/// </summary>
RopeSideVision::RopeSideVision() {
	_draw_tool = std::unique_ptr<DrawTool>();
}

/// <summary>
/// 
/// </summary>
/// <param name="input"></param>
RopeSideVision::RopeSideVision(cv::Mat &input) : RopeSideVision() {
	this->SetFrame(input);
}

/// <summary>
/// 
/// </summary>
RopeSideVision::~RopeSideVision() {
}

/// <summary>
/// 
/// </summary>
/// <param name="m"></param>
void RopeSideVision::SetFrame(cv::Mat &m) {
	_frame = m;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat RopeSideVision::GetFrame() {
	return _frame;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat RopeSideVision::GetDecoratedFrame() {
	_draw_tool = std::make_unique<DrawRopeInfos>(_decorated_frame, _lines.size(), _angle);
	_draw_tool = std::make_unique<DrawHoughLines>(_decorated_frame, _lines);
	return _decorated_frame;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat RopeSideVision::GetCannyFrame() {
	return _canny;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat RopeSideVision::GetBinaryFrame() {
	return _binary;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
double RopeSideVision::GetAngle() {
	return _angle;
}

/// <summary>
/// 
/// </summary>
void RopeSideVision::Compute() {
	cv::cvtColor(_frame, _frame, CV_RGB2GRAY);
	cv::cvtColor(_frame, _decorated_frame, CV_GRAY2BGR);
	cv::threshold(_frame, _binary, 50, 255, cv::THRESH_BINARY);
	cv::Canny(_binary, _canny, 50, 200, 3);
	this->FindLines();
	this->CalculateAngle();
}

/// <summary>
/// 
/// </summary>
void RopeSideVision::FindLines() {
	cv::Vec4i l;
	cv::Point p1, p2;
	double xmin = 3000, ymin = 3000;
	double xmax = 0, ymax = 0;

	this->_lines.clear();
	cv::HoughLinesP(_canny, _lines, 1, CV_PI / 180, 50, 50, 10);

	for (size_t i = 0; i < _lines.size(); i++) {
		l = _lines[i];
		p1 = cv::Point(l[0], l[1]); 
		p2 = cv::Point(l[2], l[3]);
	}
}

/// <summary>
/// 
/// </summary>
void RopeSideVision::CalculateAngle() {
	cv::Vec4i l;
	cv::Point p1, p2;
	double dx, dy;

	_angle = 0.0;

	for (size_t i = 0; i < _lines.size(); i++) {
		l = _lines[i];
		p1 = cv::Point(l[0], l[1]); 
		p2 = cv::Point(l[2], l[3]);
		dx = p2.x - p1.x; 
		dy = p2.y - p1.y;
		_angle += std::atan(dx / dy);
	}

	_angle = (_angle / _lines.size()) * 180 / M_PI;
}