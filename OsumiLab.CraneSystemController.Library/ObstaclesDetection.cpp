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
		this->DrawInfo(this->obstacles_, this->raw_frame_w_rects_);

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
	std::vector<std::vector<cv::Point>> contours = this->FindContoursOnFrame();
	this->obstacles_ = this->RectsToObstacles(this->CalcRotatedRects(contours));
	this->DrawObstaclesOnFrame(this->obstacles_, this->raw_frame_w_rects_);
	return this->obstacles_.size();
}

/// <summary>
/// 
/// </summary>
/// <param name="area_threshold"></param>
/// <param name="contour_area"></param>
/// <returns></returns>
std::vector<std::vector<cv::Point>> 
ObstaclesDetection::FindContoursOnFrame(uint area_threshold, uint contour_area) {
	std::vector<std::vector<cv::Point>> contours, contours_filtered;

	// Get contours from Sobel input
	cv::findContours(this->canny_frame_, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// Get obstacles contours
	for (auto it = contours.begin(); it != contours.end(); it++) 
		if (cv::contourArea(*it) > contour_area)
			contours_filtered.push_back(*it);

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
	for (auto it = contours.begin(); it != contours.end(); it++) 
		rotated_rect.push_back(cv::minAreaRect(*it));

	return rotated_rect;
}

/// <summary>
/// 
/// </summary>
/// <param name="obstacles"></param>
/// <param name="frame"></param>
void ObstaclesDetection::DrawObstaclesOnFrame(std::vector<Obstacle> obstacles, cv::Mat &frame) {
	// Get raw frame as matrix reference
	this->raw_frame_.copyTo(frame);

	// Get points from rotated rectangles
	std::vector<std::vector<cv::Point>> points;
	for (auto it = obstacles.begin(); it != obstacles.end(); it++)
		points.push_back((*it).ToPoints());

	// Draw polylines from points
	cv::polylines(frame, points,
		true, kObstaclesPolygonColor, 
		kObstaclesPolygonTickness, CV_AA);
}

/// <summary>
/// 
/// </summary>
/// <param name="rects"></param>
/// <param name="frame"></param>
void ObstaclesDetection::DrawInfo(std::vector<Obstacle> obstacles, cv::Mat &frame) {
	std::stringstream ss;
	cv::Point origin;
	cv::RotatedRect rect;

	for (auto it = obstacles.begin(); it != obstacles.end(); it++) {
		rect = (*it).GetRect();
		origin = cv::Point(rect.center.x - 10 - rect.size.height / 2, 
			rect.center.y - 10 - rect.size.width / 2);

		cv::putText(frame, (*it).ToString(), origin, 
			cv::FONT_HERSHEY_PLAIN, 0.7, cv::Scalar(0, 255, 0));
	}
}

/// <summary>
/// Method to converts vector of RotatedRect into a vector of Obstacle
/// </summary>
/// <param name="rects">Vector of RotatedRects</param>
/// <returns>Vector of Obstacle</returns>
std::vector<Obstacle> 
ObstaclesDetection::RectsToObstacles(std::vector<cv::RotatedRect> rects) {
	std::vector<Obstacle> obstacles;
	for (auto it = rects.begin(); it != rects.end(); it++) 
		obstacles.push_back(Obstacle((*it)));
	return obstacles;
}
