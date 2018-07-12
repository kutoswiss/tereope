#pragma once
#include "DrawTool.h"
#include "Obstacle.h"

class DrawRopeLoad : public DrawTool
{
public:
	// Constants
	const cv::Scalar kObstaclesPolygonColor = cv::Scalar(255, 0, 0);
	const cv::Scalar kCollisionAreaPolygonColor = cv::Scalar(0, 0, 255);

	DrawRopeLoad(cv::Mat &frame, Obstacle load);
	~DrawRopeLoad();

private:
	void Draw();

	Obstacle _load;
};

