#include "stdafx.h"
#include "DrawHoughLines.h"

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
/// <param name="lines"></param>
DrawHoughLines::DrawHoughLines(cv::Mat &frame, std::vector<cv::Vec4i> lines) {
	_lines = lines;
	this->SetFrame(frame);
}

/// <summary>
/// 
/// </summary>
DrawHoughLines::~DrawHoughLines() {
}

/// <summary>
/// 
/// </summary>
void DrawHoughLines::Draw() {
	cv::Point p1, p2;
	for(auto line = _lines.begin(); line != _lines.end(); line++) {
		p1 = cv::Point((*line)[0], (*line)[1]);
		p2 = cv::Point((*line)[2], (*line)[3]);
		cv::line(_frame, p1, p2, this->kLineColor, this->kLineTickness);
	}
}
