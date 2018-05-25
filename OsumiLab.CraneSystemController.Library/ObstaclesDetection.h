#pragma once
#include "opencv2\opencv.hpp"
#include "Obstacle.h"
#include "ObstaclesDraw.h"
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>

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
    static const uint kDefaultMinContourArea = 50;
    static const uint kBinaryThresholdValue = 35;
	static const uint kCannyThresholdValue = 255;

    /// <summary>
    /// Properties
    /// </summary>
    /// <returns></returns>
    std::vector<Obstacle> GetObstacles() const;
    cv::Mat GetCannyFrame() const;
    cv::Mat GetBinaryFrame() const;
    cv::Mat GetFrameWithRectangles();
	void SetRawFrame(cv::Mat &frame);
	void SetBinaryThreshold(uint threshold);
	void SetCannyThreshold(uint threshold);

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

    /// <summary>
    /// Private members
    /// </summary>
	ObstaclesDraw _obstacles_draw;
    
	// Vectors
    std::vector<Obstacle> _obstacles;

	// Matrices
    cv::Mat _raw_frame;
    cv::Mat _raw_frame_w_rects;
    cv::Mat _canny_frame;
    cv::Mat _bin_frame;

	// Kernels
	cv::Mat _kernel3x3;
	cv::Mat _kernel19x19;

	// Detection parameters
	uint _bin_threshold;
	uint _canny_threshold;
};