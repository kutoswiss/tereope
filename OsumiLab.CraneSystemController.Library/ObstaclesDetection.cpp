#include "stdafx.h"
#include "ObstaclesDetection.h"

/// <summary>
/// 
/// </summary>
ObstaclesDetection::ObstaclesDetection() {
}

/// <summary>
/// 
/// </summary>
ObstaclesDetection::ObstaclesDetection(cv::Mat &input) {
	this->SetRawFrame(input);
}

/// <summary>
/// 
/// </summary>
ObstaclesDetection::~ObstaclesDetection() {
}

/// <summary>
/// obstacles_ Getter
/// </summary>
/// <returns>Obstacle object</returns>
std::vector<Obstacle> ObstaclesDetection::GetObstacles() const {
	return this->obstacles_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetCannyFrame() const {
	return this->canny_frame_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetFrameWithRectangles(bool info) {
	if (info)
		this->DrawInfo(this->rotated_rects_, this->raw_frame_w_rects_);

	return this->raw_frame_w_rects_;
}

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
void ObstaclesDetection::SetRawFrame(cv::Mat &frame) {
	this->raw_frame_ = frame;
	cv::threshold(this->raw_frame_, this->binary_frame_,
		kBinaryThresholdValue, 255, cv::THRESH_BINARY);
	cv::Canny(this->binary_frame_, this->canny_frame_, kBinaryThresholdValue, 255);
}

/// <summary>
/// Method to run the detection process
/// </summary>
/// <returns>Amount of obstacles detected</returns>
size_t ObstaclesDetection::Detect() {
	std::vector<std::vector<cv::Point>> contours = this->FindContours();
	this->rotated_rects_ = this->CalcRotatedRects(contours);
	this->DrawRotatedRects(this->rotated_rects_, this->raw_frame_w_rects_);
	return this->rotated_rects_.size();
}

/// <summary>
/// 
/// </summary>
/// <param name="area_threshold"></param>
/// <param name="contour_area"></param>
/// <returns></returns>
std::vector<std::vector<cv::Point>> 
ObstaclesDetection::FindContours(unsigned int area_threshold, unsigned int contour_area) {
	std::vector<std::vector<cv::Point>> contours, contours_filtered;

	// Get contours from Sobel input
	cv::findContours(this->canny_frame_, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Get obstacles contours
	for (std::vector<std::vector<cv::Point>>::iterator it = contours.begin();
		it != contours.end(); it++) {
		if (cv::contourArea(*it) > contour_area)
			contours_filtered.push_back(*it);
	}

	return contours_filtered;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::vector<cv::RotatedRect>
ObstaclesDetection::CalcRotatedRects(std::vector<std::vector<cv::Point>> contours) {
	std::vector<cv::RotatedRect> rotated_rect;

	// Calculate obstacles rotated rectangles
	std::vector<cv::RotatedRect> obstaclesRotatedRects;
	for (std::vector<std::vector<cv::Point>>::iterator it = contours.begin();
		it != contours.end(); it++) {
		rotated_rect.push_back(cv::minAreaRect(*it));
	}

	return rotated_rect;
}

void ObstaclesDetection::DrawRotatedRects(std::vector<cv::RotatedRect> rects, cv::Mat &frame) {
	// Get raw frame as matrix reference
	this->raw_frame_.copyTo(frame);

	// Get points from rotated rectangles
	std::vector<std::vector<cv::Point>> points;
	for (std::vector<cv::RotatedRect>::iterator it = rects.begin();
		it != rects.end(); it++) {
		cv::Point2f vertices[4];
		std::vector<cv::Point> pts;

		(*it).points(vertices);
		for (int i = 0; i < 4; i++) 
			pts.push_back(vertices[i]);

		points.push_back(pts);
	}

	// Draw polylines from points
	cv::polylines(frame, points,
		true, kObstaclesPolygonColor, kObstaclesPolygonTickness, CV_AA);
}

/// <summary>
/// 
/// </summary>
/// <param name="rects"></param>
/// <param name="frame"></param>
void ObstaclesDetection::DrawInfo(std::vector<cv::RotatedRect> rects, cv::Mat &frame) {
	for (std::vector<cv::RotatedRect>::iterator it = rects.begin();
		it != rects.end(); it++) {
		std::stringstream ss;
		ss << "Origin: " << (int)((*it).center.x) << "," << (int)((*it).center.y) 
			<< "; Angle: " << std::setprecision(2) << (*it).angle << " deg.";

		cv::Point origin((*it).center.x - 10 - (*it).size.height/2, (*it).center.y - 10 - (*it).size.width/2);
		cv::putText(frame, ss.str(), origin, cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(0, 255, 0));
	}
}
