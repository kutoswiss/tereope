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
//
///// <summary>
///// 
///// </summary>
///// <param name="obstacle"></param>
///// <returns></returns>
//bool ObstaclesDetection::CollideWithRopeLoadArea(Obstacle obstacle) {
//	auto pts1 = obstacle.ToPoints();
//	std::vector<cv::Point> pts2;
//	pts2.push_back(cv::Point(this->_rope_load_area.x, this->_rope_load_area.y));
//	pts2.push_back(cv::Point(this->_rope_load_area.x, 
//		this->_rope_load_area.y + this->_rope_load_area.Height));
//	pts2.push_back(cv::Point(this->_rope_load_area.x + this->_rope_load_area.Width, 
//		this->_rope_load_area.y));
//	pts2.push_back(cv::Point(this->_rope_load_area.x + this->_rope_load_area.Width,
//		this->_rope_load_area.y + this->_rope_load_area.Height));
//
//	return this->LinesIntersect(pts1, pts2);
//}
//
///// <summary>
///// 
///// </summary>
///// <param name="l1"></param>
///// <param name="l2"></param>
///// <returns></returns>
//bool ObstaclesDetection::LineIntersect(std::tuple<cv::Point, cv::Point> l1, std::tuple<cv::Point, cv::Point> l2) {
//	double x1_0 = std::get<0>(l1).x;
//	double y1_0 = std::get<0>(l1).y;
//	double x2_0 = std::get<1>(l1).x;
//	double y2_0 = std::get<1>(l1).y;
//
//	double x1_1 = std::get<0>(l2).x;
//	double y1_1 = std::get<0>(l2).y;
//	double x2_1 = std::get<1>(l2).x;
//	double y2_1 = std::get<1>(l2).y;
//
//	double A0 = y2_0 - y1_0;
//	double B0 = x1_0 - x2_0;
//	double C0 = A0*x1_0 + B0*y1_0;
//
//	double A1 = y2_1 - y1_1;
//	double B1 = x1_1 - x2_1;
//	double C1 = A1*x1_1 + B0*y1_1;
//
//	double det = A0 * B1 - A1 * B0;
//
//	if (det != 0) {
//		double x_intersect = (B1*C0 - B0*C1) / det;
//		double y_intersect = (A0*C1 - A1*C0) / det;
//
//		bool x_condition = (x_intersect >= (x1_1 - 5)) && (x_intersect <= (x2_1 + 5));
//			//&& (x_intersect >= (x1_0 - 5)) && (x_intersect <= (x2_0 + 5));
//
//		bool y_condition = (y_intersect >= (y1_1 - 5)) && (y_intersect <= (y2_1 + 5));
//			//&& (y_intersect >= (y1_0 - 5)) && (y_intersect <= (y2_0 + 5));
//
//		return (x_condition && y_condition);
//	} else 
//		return false;
//}
//
///// <summary>
///// 
///// </summary>
///// <param name="p1"></param>
///// <param name="p2"></param>
///// <returns></returns>
//bool ObstaclesDetection::LinesIntersect(std::vector<cv::Point> p1, std::vector<cv::Point> p2) {
//	auto lines_1 = this->GetLines(p1);
//	auto lines_2 = this->GetLines(p2);
//	bool res = false;
//
//	for (auto l1 = lines_1.begin(); l1 < lines_1.end(); l1++) {
//		for (auto l2 = lines_2.begin(); l2 < lines_2.end(); l2++) {
//			res |= this->LineIntersect((*l1), (*l2));
//		}
//	}
//	
//	return res;
//}

///// <summary>
///// 
///// </summary>
///// <param name="pts"></param>
///// <returns></returns>
//std::vector<std::tuple<cv::Point, cv::Point>>
//ObstaclesDetection::GetLines(std::vector<cv::Point> pts) {
//	std::vector<std::tuple<cv::Point, cv::Point>> lines;
//
//	for (auto p = pts.begin(); p != pts.end(); p++) {
//		for (auto tmp = pts.begin(); tmp < pts.end(); tmp++) {
//			if ((*p) != (*tmp))
//				lines.push_back(std::make_tuple((*p), (*tmp)));
//		}
//	}
//
//	return lines;
//}
//
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
/// <param name="l1"></param>
/// <param name="l2"></param>
/// <returns></returns>
bool ObstaclesDetection::SegmentsIntersection(cv::Point p, cv::Point pr, cv::Point q, cv::Point qs) {
	cv::Point cmp(q.x - p.x, q.y - p.y);
	cv::Point r(pr.x - p.x, pr.y - p.y);
	cv::Point s(qs.x - q.x, qs.y - q.y);
	double cmp_xr = cmp.x * r.y - cmp.y * r.x;
	double cmp_xs = cmp.x * s.y - cmp.y * s.x;
	double rxs = r.x * s.y - r.y * s.x;
	float rxsr = 1.0 / rxs;
	float t = cmp_xs * rxsr;
	float u = cmp_xr * rxsr;

	if (rxsr == 0)
		return false;

	if (cmp_xr == 0) 
		return ((q.x - p.x < 0) != (q.x - pr.x < 0)) 
		|| ((q.y - p.y < 0) != (q.y - pr.y < 0));

	return (t >= 0) && (t <= 1) && (u >= 0) && (u <= 1);
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool ObstaclesDetection::CollideWithRopeLoadArea() {
	auto seg_area = _rope_load.ToSegments();
	bool res = false;

	for (size_t i = 0; i < _obstacles.size(); i++) {
		auto seg_obstacles = _obstacles[i].ToSegments();
		for (size_t j = 0; j < seg_obstacles.size(); j++)
		{
			for (size_t k = 0; k < seg_area.size(); k++)
			{
				cv::Point p = std::get<0>(seg_obstacles[j]);
				cv::Point pr = std::get<1>(seg_obstacles[j]);
				cv::Point q = std::get<0>(seg_area[k]);
				cv::Point qs = std::get<1>(seg_area[k]);

				res |= this->SegmentsIntersection(p, pr, q, qs);
			}
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

	for (auto o = _obstacles.begin(); o != _obstacles.end(); o++)
		res |= _rope_load.CollideWith(*o);

	return res;
}

