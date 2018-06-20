#include "stdafx.h"
#include "RopeVision.h"


RopeVision::RopeVision() {
}

RopeVision::RopeVision(cv::Mat &input) {
	this->SetFrame(input);
}

RopeVision::~RopeVision() {
}

void RopeVision::SetFrame(cv::Mat &m) {
	_frame = m;
}

cv::Mat RopeVision::GetFrame() {
	return _frame;
}

cv::Mat RopeVision::GetFrameWithLines() {
	return _frame_w_lines;
}

void RopeVision::Compute() {
	cv::cvtColor(_frame, _frame, CV_RGB2GRAY);
	cv::threshold(_frame, _frame, 50, 255, cv::THRESH_BINARY);
	cv::Canny(_frame, _frame, 50, 200, 3);
	cv::cvtColor(_frame, _frame_w_lines, CV_GRAY2BGR);

	this->_lines.clear();
	cv::HoughLinesP(_frame, _lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < _lines.size(); i++) {
		cv::Vec4i l = _lines[i];
		cv::line(_frame_w_lines, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0, 255, 0), 2, CV_AA);
	}
}