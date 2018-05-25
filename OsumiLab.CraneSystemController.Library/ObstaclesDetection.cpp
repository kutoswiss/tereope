#include "stdafx.h"
#include "ObstaclesDetection.h"

/// <summary>
/// 
/// </summary>
ObstaclesDetection::ObstaclesDetection() {
	this->_bin_threshold = this->kBinaryThresholdValue;
	this->_canny_threshold = this->kCannyThresholdValue;

	this->_kernel19x19 = cv::getStructuringElement(cv::MORPH_RECT,
		cv::Size(2 * 9 + 1, 2 * 9 + 1), cv::Point(9, 9));

	this->_kernel3x3 = cv::getStructuringElement(cv::MORPH_RECT,
		cv::Size(3, 3));
}

/// <summary>
/// 
/// </summary>
ObstaclesDetection::ObstaclesDetection(cv::Mat &input) : ObstaclesDetection() {
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
    return this->_obstacles;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetCannyFrame() const {
    return this->_canny_frame;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetBinaryFrame() const {
    return this->_bin_frame;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetFrameWithRectangles() {
	this->_raw_frame.copyTo(this->_raw_frame_w_rects);
	this->_obstacles_draw.SetFrame(this->_raw_frame_w_rects);
	this->_obstacles_draw.Draw(this->_obstacles);
    return this->_raw_frame_w_rects;
}

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
void ObstaclesDetection::SetRawFrame(cv::Mat &frame) {
    this->_raw_frame = frame;
	cv::morphologyEx(this->_raw_frame, this->_bin_frame, cv::MORPH_OPEN, this->_kernel19x19);
    cv::threshold(this->_bin_frame, this->_bin_frame, this->_bin_threshold, 255, cv::THRESH_BINARY);
	cv::dilate(this->_bin_frame, this->_bin_frame, this->_kernel3x3);
    cv::cvtColor(this->_bin_frame, this->_bin_frame, CV_RGB2GRAY);
    cv::Canny(this->_bin_frame, this->_canny_frame, this->_canny_threshold, 255);
}

/// <summary>
/// 
/// </summary>
/// <param name="threshold"></param>
void ObstaclesDetection::SetBinaryThreshold(uint threshold) {
	if (threshold > 255)
		threshold = 255;

	this->_bin_threshold = threshold;
}

/// <summary>
/// 
/// </summary>
/// <param name="threshold"></param>
void ObstaclesDetection::SetCannyThreshold(uint threshold) {
	if (threshold > 255)
		threshold = 255;

	this->_canny_threshold = threshold;
}

/// <summary>
/// Method to run the detection process
/// </summary>
/// <returns>Amount of obstacles detected</returns>
size_t ObstaclesDetection::Detect(bool print_detect) {
    std::vector<std::vector<cv::Point>> contours = this->FindContoursOnFrame();
    this->_obstacles = this->RectsToObstacles(this->CalcRotatedRects(contours));
	if(print_detect) 
		this->PrintDetect();
    return this->_obstacles.size();
}

/// <summary>
/// Print on console the obstacles detection informations
/// </summary>
void ObstaclesDetection::PrintDetect() {
	std::cout << std::endl << this->_obstacles.size() << " obstacles detected." << std::endl;
	for (auto o = this->_obstacles.begin(); o != this->_obstacles.end(); o++)
		std::cout << (*o).ToString() << std::endl;
}

/// <summary>
/// 
/// </summary>
/// <param name="area_threshold"></param>
/// <param name="contour_area"></param>
/// <returns></returns>
std::vector<std::vector<cv::Point>> 
ObstaclesDetection::FindContoursOnFrame(uint min_contour_area) {
    std::vector<std::vector<cv::Point>> contours, contours_filtered;

    // Get contours from Binary input
    cv::findContours(this->_bin_frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Get obstacles contours
	for (auto it = contours.begin(); it != contours.end(); it++)
		if (cv::contourArea(*it) > min_contour_area)
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
