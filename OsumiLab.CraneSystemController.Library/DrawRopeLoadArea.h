#pragma once
#include "DrawTool.h"
#include "RopeLoadArea.h"

class DrawRopeLoadArea : public DrawTool
{
public:
	// Constants
	const cv::Scalar kObstaclesPolygonColor = cv::Scalar(0, 0, 255);

	DrawRopeLoadArea(cv::Mat &frame, RopeLoadArea rope_load_area);
	~DrawRopeLoadArea();

private:
	void Draw();

	RopeLoadArea _rope_load_area;
};

