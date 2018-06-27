#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "RopeLoadArea.h"

/// <summary>
/// Default constructor
/// </summary>
RopeLoadArea::RopeLoadArea() {
}

/// <summary>
/// Deletage constructor
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="center"></param>
RopeLoadArea::RopeLoadArea(double width, double height, cv::Point center) {
}

/// <summary>
/// Destructor
/// </summary>
RopeLoadArea::~RopeLoadArea() {
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
double RopeLoadArea::X() const {
	return _rect.center.x - (_rect.size.width / 2);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
double RopeLoadArea::Y() const {
	return _rect.center.y - (_rect.size.height / 2);
}

/// <summary>
/// Get width
/// </summary>
/// <returns></returns>
double RopeLoadArea::Width() const {
	return _rect.size.width;
}

/// <summary>
/// Get height
/// </summary>
/// <returns></returns>
double RopeLoadArea::Height() const {
	return _rect.size.height;
}

/// <summary>
/// Get center point
/// </summary>
/// <returns></returns>
cv::Point RopeLoadArea::Center() const {
	return _rect.center;
}

/// <summary>
/// 
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="center"></param>
void RopeLoadArea::Set(double width, double height, cv::Point center) {
	_rect.size.width = width;
	_rect.size.height = height;
	_rect.center.x = center.x;
	_rect.center.y = center.y;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::vector<cv::Point> RopeLoadArea::ToPoints() {
	const uint kVerticesSize = 4;
	cv::Point2f vertices[kVerticesSize];
	std::vector<cv::Point> pts;

	_rect.points(vertices);
	for (int i = 0; i < kVerticesSize; i++)
		pts.push_back(vertices[i]);

	return pts;
}
