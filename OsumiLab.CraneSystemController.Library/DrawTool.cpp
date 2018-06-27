#include "stdafx.h"
#include "DrawTool.h"


DrawTool::DrawTool() {
}

DrawTool::~DrawTool() {
}

void DrawTool::SetFrame(cv::Mat &frame) {
	_frame = frame;
	this->Draw();
}

cv::Mat DrawTool::GetFrame() {
	return _frame;
}