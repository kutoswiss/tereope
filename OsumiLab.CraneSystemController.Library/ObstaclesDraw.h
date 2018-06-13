#pragma once
#include "opencv2\opencv.hpp"
#include "Obstacle.h"
#include <iostream>

class ObstaclesDraw
{
public:
	ObstaclesDraw();
	ObstaclesDraw(cv::Mat &frame);
	~ObstaclesDraw();

	/// <summary>
	/// Constants
	/// </summary>
	static const uint kObstaclesPolygonTickness = 2; 
	const cv::Scalar kObstaclesPolygonColor = cv::Scalar(0, 255, 0);
	const cv::Scalar kObstaclesOriginColor = cv::Scalar(0, 0, 255);
	const cv::Scalar kObstaclesCornerColor = cv::Scalar(255, 0, 0);

	/// <summary>
	/// Public methods
	/// </summary>
	void SetFrame(cv::Mat &frame);
	void Draw(std::vector<Obstacle> obstacles, uint tickness = kObstaclesPolygonTickness);
	void DrawObstacle(Obstacle obstacle, cv::Scalar color, uint tickness = kObstaclesPolygonTickness);
	void DrawObstacles(std::vector<Obstacle> obstacles, uint tickness = kObstaclesPolygonTickness);
	void DrawAdditionnalInfos(std::vector<Obstacle> obstacles);

private:
	/// <summary>
	/// Private methods
	/// </summary>
	/// <param name="obstacle"></param>
	void DrawTextInfos(Obstacle obstacle);
	void DrawOrigin(Obstacle obstacle);
	void DrawCorners(Obstacle obstacle);
	void DrawRopeLoadArea(cv::Rect2d r);

	/// <summary>
	/// Private members
	/// </summary>
	cv::Mat _frame;
	bool _textinfos;
	bool _origins;
	bool _corners;
};

