#pragma once
#include "DrawTool.h"
#include "Obstacle.h"

class DrawObstaclesInfos : public DrawTool
{
public:
	// Constants
	const cv::Scalar kTextColor = cv::Scalar(255, 255, 255);

	DrawObstaclesInfos(cv::Mat &frame, std::vector<Obstacle> obstacles);
	~DrawObstaclesInfos();

private:
	void Draw();
	void DrawInfos(std::vector<Obstacle> obstacles);

	std::vector<Obstacle> _obstacles;
};

