#pragma once
#include "DrawTool.h"
#include "Obstacle.h"

class DrawObstacles : public DrawTool
{
public:
	// Constants
	const cv::Scalar kObstaclesPolygonColor = cv::Scalar(0, 255, 0);

	// Ctor/Dtor
	DrawObstacles(cv::Mat &frame, std::vector<Obstacle> obstacles);
	~DrawObstacles();

private:
	void Draw();
	void DrawObstaclesOnFrame(std::vector<Obstacle> obstacles, uint tickness);

	std::vector<Obstacle> _obstacles;
};

