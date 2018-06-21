#include "stdafx.h"
#include "RopeSideVision.h"


RopeSideVision::RopeSideVision() {
}

RopeSideVision::RopeSideVision(cv::Mat &input) {
	this->SetFrame(input);
}

RopeSideVision::~RopeSideVision() {
}

void RopeSideVision::SetFrame(cv::Mat &m) {
	_frame = m;
}

cv::Mat RopeSideVision::GetFrame() {
	return _frame;
}

cv::Mat RopeSideVision::GetFrameWithLines() {
	return _frame_w_lines;
}

cv::Mat RopeSideVision::GetCannyFrame() {
	return _canny;
}

double RopeSideVision::GetAngle() {
	return _angle;
}

void RopeSideVision::Compute() {
	cv::cvtColor(_frame, _frame, CV_RGB2GRAY);
	cv::cvtColor(_frame, _frame_w_lines, CV_GRAY2BGR);
	cv::threshold(_frame, _frame, 50, 255, cv::THRESH_BINARY);
	cv::Canny(_frame, _canny, 50, 200, 3);
	this->FindLines();
	this->CalculateAngle();
}

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

		if (p1.x < xmin) xmin = p1.x;
		if (p2.x < xmin) xmin = p2.x;
		if (p1.x > xmax) xmax = p1.x;
		if (p2.x > xmax) xmax = p2.x;

		if (p1.y < ymin) ymin = p1.y;
		if (p2.y < ymin) ymin = p2.y;
		if (p1.y > ymax) ymax = p1.y;
		if (p2.y > ymax) ymax = p2.y;

		//cv::line(_frame_w_lines, p1, p2, cv::Scalar(0, 255, 0), 2, CV_AA);
	}

	cv::line(_frame_w_lines, 
		cv::Point(xmin, ymin), cv::Point(xmax, ymax), 
		cv::Scalar(0, 0, 255), 2, CV_AA);

}

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
		_angle += std::atan(dy / dx);
	}

	_angle = (_angle / _lines.size()) * 180 / M_PI;
}