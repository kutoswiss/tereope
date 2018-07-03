#include "stdafx.h"
#include "DrawTool.h"

/// <summary>
/// 
/// </summary>
DrawTool::DrawTool() {
}

/// <summary>
/// 
/// </summary>
DrawTool::~DrawTool() {
}

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
void DrawTool::SetFrame(cv::Mat &frame) {
	_frame = frame;
	this->Draw();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat DrawTool::GetFrame() {
	return _frame;
}