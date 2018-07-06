#pragma once
#include "opencv2\opencv.hpp"
#include "Obstacle.h"
#include "ObstaclesDraw.h"
#include "CameraHelper.h"
#include "RopeLoadArea.h"
#include "DrawTool.h"
#include "DrawObstacles.h"
#include "DrawObstaclesCorners.h"
#include "DrawObstaclesInfos.h"
#include "DrawRopeLoadArea.h"
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>
#include <memory>

using namespace AVT::VmbAPI;

class ObstaclesDetection
{
public:
    /// <summary>
    /// Ctor/Dtor
    /// </summary>
    ObstaclesDetection();
    ObstaclesDetection(cv::Mat &input);
    ~ObstaclesDetection();

    /// <summary>
    /// Constants
    /// </summary>
    static const uint kDefaultMinContourArea = 200;
    static const uint kBinaryThresholdValue = 45;
	static const uint kCannyThresholdValue = 255;

    /// <summary>
    /// Properties
    /// </summary>
    /// <returns></returns>
    std::vector<Obstacle> GetObstacles() const;
	Obstacle GetRopeLoad() const;
    cv::Mat GetCannyFrame() const;
    cv::Mat GetBinaryFrame() const;
    cv::Mat GetFrameWithRectangles();
	bool IsCollided() const;
	void SetRawFrame(cv::Mat &frame);
	void SetBinaryThreshold(uint threshold);
	void SetCannyThreshold(uint threshold);

	bool IsInsideRopeLoadArea(cv::RotatedRect rect);
	bool CollideWithRopeLoadArea();
	bool RopeLoadCollidesWithObstacles();

	bool LineIntersect(std::tuple<cv::Point, cv::Point> l1, std::tuple<cv::Point, cv::Point> l2);
	//bool LinesIntersect(std::vector<cv::Point> p1, std::vector<cv::Point> p2);
	bool SegmentsIntersection(cv::Point p, cv::Point pr, cv::Point q, cv::Point qs);
	std::vector<std::tuple<cv::Point, cv::Point>> GetLines(std::vector<cv::Point> pts);

    /// <summary>
    /// Public methods
    /// </summary>
    /// <returns></returns>
    size_t Detect(bool print_detect = false);
	void PrintDetect();

private:
    /// <summary>
    /// Private methods
    /// </summary>
    std::vector<std::vector<cv::Point>>
		FindContoursOnFrame(uint min_contour_area = kDefaultMinContourArea);
    std::vector<cv::RotatedRect> 
		CalcRotatedRects(std::vector<std::vector<cv::Point>> contours);
    std::vector<Obstacle>
		RectsToObstacles(std::vector<cv::RotatedRect> rects);
	bool ObstaclesInsideRopeArea(void);


    /// <summary>
    /// Private members
    /// </summary>
	ObstaclesDraw _obstacles_draw;
	std::unique_ptr<DrawTool> _draw_tool;
    
	// Vectors
    std::vector<Obstacle> _obstacles;

	// Rope load area
	//cv::Rect2d _rope_load_area;
	RopeLoadArea _rope_load_area;
	Obstacle _rope_load;

	// Matrices
    cv::Mat _raw_frame;
    cv::Mat _raw_frame_w_rects;
    cv::Mat _canny_frame;
    cv::Mat _bin_frame;

	// Kernels
	cv::Mat _kernel3;
	cv::Mat _kernel10;

	// Detection parameters
	uint _bin_threshold;
	uint _canny_threshold;
	bool _collide = false;
};