#pragma once
#include "DrawTool.h"
#include "Obstacle.h"

class DrawObstaclesCorners : public DrawTool
{
public:
	// Constants
	const cv::Scalar kObstacleCornerColor = cv::Scalar(255, 0, 0);

	DrawObstaclesCorners(cv::Mat &frame, std::vector<Obstacle> obstacles);
	~DrawObstaclesCorners();

private:
	void Draw();
	void DrawCorners(std::vector<Obstacle> obstacles, uint tickness);

	std::vector<Obstacle> _obstacles;
};

