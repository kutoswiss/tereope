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
#include "DrawRopeLoad.h"
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>
#include <memory>

using namespace AVT::VmbAPI;

class ObstaclesDetection
{
public:
    // Ctor/Dtor
    ObstaclesDetection();
    ObstaclesDetection(cv::Mat &input);
    ~ObstaclesDetection();

    // Constants
    static const uint kDefaultMinContourArea = 200;
    static const uint kBinaryThresholdValue = 45;
	static const uint kCannyThresholdValue = 255;

    // Getters/Setters
    std::vector<Obstacle> GetObstacles() const;
	Obstacle GetRopeLoad() const;
    cv::Mat GetCannyFrame() const;
    cv::Mat GetBinaryFrame() const;
	cv::Mat GetOpenFrame() const;
    cv::Mat GetFrameWithRectangles();
	bool IsCollided() const;
	void SetRawFrame(cv::Mat &frame);
	void SetBinaryThreshold(uint threshold);
	void SetCannyThreshold(uint threshold);
	void SetRopeLoadAreaOrigin(cv::Point origin);

	bool IsInsideRopeLoadArea(cv::RotatedRect rect);
	bool RopeLoadCollidesWithObstacles();
	int RopeAreaCollidesWithObstacles();

    // Public methods
    size_t Detect(bool print_detect = false);
	void PrintDetect();

private:
    // Private methods
    std::vector<std::vector<cv::Point>>
		FindContoursOnFrame(uint min_contour_area = kDefaultMinContourArea);
    std::vector<cv::RotatedRect> 
		CalcRotatedRects(std::vector<std::vector<cv::Point>> contours);
    std::vector<Obstacle>
		RectsToObstacles(std::vector<cv::RotatedRect> rects);
	bool ObstaclesInsideRopeArea(void);
	bool TrackRopeLoad(cv::RotatedRect rect);


    /// <summary>
    /// Private members
    /// </summary>
	ObstaclesDraw _obstacles_draw;
	std::unique_ptr<DrawTool> _draw_tool;
    
	// Vectors
    std::vector<Obstacle> _obstacles;

	// Rope load area
	RopeLoadArea _rope_load_area;
	Obstacle _rope_load;

	// Matrices
    cv::Mat _raw_frame;
    cv::Mat _raw_frame_w_rects;
    cv::Mat _canny_frame;
    cv::Mat _bin_frame;
	cv::Mat _open_frame;

	// Kernels
	cv::Mat _kernel3;
	cv::Mat _kernel10;

	// Detection parameters
	uint _bin_threshold;
	uint _canny_threshold;
	bool _collide = false;
};