#include "stdafx.h"
#include "ObstacleCollisionArea.h"

/// <summary>
/// 
/// </summary>
ObstacleCollisionArea::ObstacleCollisionArea() {
}

/// <summary>
/// 
/// </summary>
/// <param name="collision_area"></param>
ObstacleCollisionArea::ObstacleCollisionArea(cv::RotatedRect collision_area) {
	_collision_area = collision_area;
}

/// <summary>
/// 
/// </summary>
ObstacleCollisionArea::~ObstacleCollisionArea() {
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::RotatedRect ObstacleCollisionArea::GetRect() const {
	return _collision_area;
}

/// <summary>
/// 
/// </summary>
/// <param name="center"></param>
/// <param name="angle"></param>
/// <param name="width"></param>
/// <param name="height"></param>
void ObstacleCollisionArea::SetArea(cv::Point center, double angle, int width, int height) {
	_collision_area.center = center;
	_collision_area.angle = angle;
	_collision_area.size.width = width;
	_collision_area.size.height = height;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::vector<cv::Point> ObstacleCollisionArea::ToPoints() {
	const uint kVerticesSize = 4;
	cv::Point2f vertices[kVerticesSize];
	std::vector<cv::Point> pts;

	_collision_area.points(vertices);
	for (int i = 0; i < kVerticesSize; i++)
		pts.push_back(vertices[i]);

	return pts;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::vector<std::tuple<cv::Point, cv::Point>> ObstacleCollisionArea::ToSegments() {
	/*std::vector<cv::Point> points = this->ToPoints();
	std::vector<std::tuple<cv::Point, cv::Point>> segments;
	cv::Point tmp_pt;

	while (points.size() > 1) {
		tmp_pt = points.back();
		points.pop_back();

		for (auto p = points.begin(); p != points.end(); p++)
			segments.push_back(std::make_tuple(tmp_pt, (*p)));
	}

	return segments;*/

	std::vector<cv::Point> points = this->ToPoints();
	std::vector<std::tuple<cv::Point, cv::Point>> segments;

	segments.push_back(std::make_tuple(points[0], points[1]));
	segments.push_back(std::make_tuple(points[1], points[2]));
	segments.push_back(std::make_tuple(points[2], points[3]));
	segments.push_back(std::make_tuple(points[3], points[0]));

	return segments;
}

/// <summary>
/// 
/// </summary>
/// <param name="area"></param>
/// <returns></returns>
bool ObstacleCollisionArea::CollideWith(ObstacleCollisionArea area) {
	auto segments_1 = this->ToSegments();
	auto segments_2 = area.ToSegments();
	bool res = false;
	cv::Point p, pr, q, qs;

	for(auto s1 = segments_1.begin(); s1 != segments_1.end(); s1++) {
		for (auto s2 = segments_2.begin(); s2 != segments_2.end(); s2++) {
			p = std::get<0>(*s1);
			pr = std::get<1>(*s1);
			q = std::get<0>(*s2);
			qs = std::get<1>(*s2);
			res |= this->SegmentsIntersects(p, pr, q, qs);
		}
	}

	//cv::Point area_center = area.GetRect().center;
	//std::vector<cv::Point> area_points = area.ToPoints();
	//if(area_center.x > )

	return res;
}

/// <summary>
/// 
/// </summary>
/// <param name="p"></param>
/// <param name="pr"></param>
/// <param name="q"></param>
/// <param name="qs"></param>
/// <returns></returns>
bool ObstacleCollisionArea::SegmentsIntersects(cv::Point p, cv::Point pr, cv::Point q, cv::Point qs) {
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

