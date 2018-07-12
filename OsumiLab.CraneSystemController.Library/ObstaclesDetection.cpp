#include "stdafx.h"
#include "ObstaclesDetection.h"

/// <summary>
/// 
/// </summary>
ObstaclesDetection::ObstaclesDetection() {
	_bin_threshold = kBinaryThresholdValue;
	_canny_threshold = kCannyThresholdValue;

	_kernel3 = CameraHelper::GetOnesKernel(3);
	_kernel10 = CameraHelper::GetOnesKernel(10);

	_draw_tool = std::unique_ptr<DrawTool>();

	// Define rope load area
	_rope_load_area.Set(100, 100, cv::Point(300, 325));
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
    return _obstacles;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
Obstacle ObstaclesDetection::GetRopeLoad() const {
	return _rope_load;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetCannyFrame() const {
    return _canny_frame;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetBinaryFrame() const {
    return _bin_frame;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat ObstaclesDetection::GetFrameWithRectangles() {
	_raw_frame.copyTo(_raw_frame_w_rects);
	_draw_tool = std::make_unique<DrawObstacles>(_raw_frame_w_rects, _obstacles);
	_draw_tool = std::make_unique<DrawObstaclesCorners>(_raw_frame_w_rects, _obstacles);
	_draw_tool = std::make_unique<DrawObstaclesInfos>(_raw_frame_w_rects, _obstacles);
	_draw_tool = std::make_unique<DrawRopeLoadArea>(_raw_frame_w_rects, _rope_load_area);
	_draw_tool = std::make_unique<DrawRopeLoad>(_raw_frame_w_rects, _rope_load);

	_collide = this->ObstaclesInsideRopeArea();
    return _raw_frame_w_rects;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool ObstaclesDetection::IsCollided() const {
	return _collide;
}

/// <summary>
/// 
/// </summary>
/// <param name="frame"></param>
void ObstaclesDetection::SetRawFrame(cv::Mat &frame) {
    _raw_frame = frame;
	cv::morphologyEx(_raw_frame, _bin_frame, cv::MORPH_OPEN, _kernel10);
    cv::threshold(_bin_frame, _bin_frame, _bin_threshold, 255, cv::THRESH_BINARY);
	cv::dilate(_bin_frame, _bin_frame, _kernel3);
    cv::cvtColor(_bin_frame, _bin_frame, CV_RGB2GRAY);
    cv::Canny(_bin_frame, _canny_frame, _canny_threshold, 255);
}

/// <summary>
/// 
/// </summary>
/// <param name="threshold"></param>
void ObstaclesDetection::SetBinaryThreshold(uint threshold) {
	if (threshold > 255)
		threshold = 255;

	_bin_threshold = threshold;
}

/// <summary>
/// 
/// </summary>
/// <param name="threshold"></param>
void ObstaclesDetection::SetCannyThreshold(uint threshold) {
	if (threshold > 255)
		threshold = 255;

	_canny_threshold = threshold;
}

/// <summary>
/// 
/// </summary>
/// <param name="origin"></param>
void ObstaclesDetection::SetRopeLoadAreaOrigin(cv::Point origin) {
	_rope_load_area.Set(100, 100, origin);
}

/// <summary>
/// Method to run the detection process
/// </summary>
/// <returns>Amount of obstacles detected</returns>
size_t ObstaclesDetection::Detect(bool print_detect) {
    std::vector<std::vector<cv::Point>> contours = this->FindContoursOnFrame();
    _obstacles = this->RectsToObstacles(this->CalcRotatedRects(contours));
	
	if(print_detect) 
		this->PrintDetect();
	
    return _obstacles.size();
}

/// <summary>
/// Print on console the obstacles detection informations
/// </summary>
void ObstaclesDetection::PrintDetect() {
	std::cout << std::endl << _obstacles.size() << " obstacles detected." << std::endl;
	for (auto o = _obstacles.begin(); o != _obstacles.end(); o++)
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
    cv::findContours(_bin_frame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

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
    for (auto it = rects.begin(); it != rects.end(); it++) {
		if (this->IsInsideRopeLoadArea(*it))
			_rope_load = Obstacle(*it);
		else
			obstacles.push_back(Obstacle((*it)));
	}
    return obstacles;
}

/// <summary>
/// 
/// </summary>
/// <param name="rect"></param>
/// <returns></returns>
bool ObstaclesDetection::IsInsideRopeLoadArea(cv::RotatedRect rect) {
	bool x_condition = ((rect.center.x > _rope_load_area.X()) && 
		(rect.center.x < (_rope_load_area.X() + _rope_load_area.Width())));
	bool y_condition = ((rect.center.y > _rope_load_area.Y()) &&
		(rect.center.y < (_rope_load_area.Y() + _rope_load_area.Height())));

	bool width_condition = (rect.size.width < (_rope_load_area.Width() / 2));
	bool height_condition = (rect.size.height < (_rope_load_area.Height() / 2));

	return x_condition && y_condition && width_condition && height_condition;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool ObstaclesDetection::ObstaclesInsideRopeArea(void) {
	bool res = false;
	int x_origin = _rope_load_area.X();
	int y_origin = _rope_load_area.Y();

	for (int y = y_origin; y < (_rope_load_area.Height() + y_origin); y++) {
		for (int x = x_origin; x < (_rope_load_area.Width() + x_origin); x++) {
			cv::Vec3b c = _raw_frame_w_rects.at<cv::Vec3b>(y, x);
			res |= (c.val[1] >= 240);
		}
	}

	return res;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool ObstaclesDetection::RopeLoadCollidesWithObstacles() {
	bool res = false;

	for (auto o = _obstacles.begin(); o != _obstacles.end(); o++) {
		if(_rope_load.CollideWith(*o)) {
			res = true;
			break;
		}
	}

	return res;
}

