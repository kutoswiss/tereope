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

void RopeSideVision::Compute() {
	cv::cvtColor(_frame, _frame, CV_RGB2GRAY);
	cv::threshold(_frame, _frame, 50, 255, cv::THRESH_BINARY);
	cv::Canny(_frame, _frame, 50, 200, 3);
	cv::cvtColor(_frame, _frame_w_lines, CV_GRAY2BGR);
	this->FindLines();
}

void RopeSideVision::FindLines() {
	cv::Vec4i l;
	cv::Point p1, p2;

	this->_lines.clear();
	cv::HoughLinesP(_frame, _lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < _lines.size(); i++) {
		l = _lines[i];
		p1 = cv::Point(l[0], l[1]); p2 = cv::Point(l[2], l[3]);
		cv::line(_frame_w_lines, p1, p2, cv::Scalar(0, 255, 0), 2, CV_AA);
	}
}

void RopeSideVision::CalculateAngle() {

}