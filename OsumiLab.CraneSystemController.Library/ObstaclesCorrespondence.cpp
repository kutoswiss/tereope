#include "stdafx.h"
#include "ObstaclesCorrespondence.h"

/// <summary>
/// 
/// </summary>
ObstaclesCorrespondence::ObstaclesCorrespondence() {
}

/// <summary>
/// 
/// </summary>
/// <param name="left_samples"></param>
/// <param name="right_samples"></param>
ObstaclesCorrespondence::ObstaclesCorrespondence(
	std::vector<Obstacle> left_samples, std::vector<Obstacle> right_samples) {
	this->SetSamples(left_samples, right_samples);
}

/// <summary>
/// 
/// </summary>
ObstaclesCorrespondence::~ObstaclesCorrespondence() {
}

/// <summary>
/// 
/// </summary>
/// <param name="left_samples"></param>
/// <param name="right_samples"></param>
void ObstaclesCorrespondence::SetSamples(
	std::vector<Obstacle> left_samples, std::vector<Obstacle> right_samples) {
	this->_left_samples = left_samples;
	this->_right_samples = right_samples;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
std::vector<StereoObstacle> ObstaclesCorrespondence::Match(void) {
	for(auto l = this->_left_samples.begin(); l != this->_left_samples.end(); l++) {
		for (auto r = this->_right_samples.begin(); r != this->_right_samples.end(); r++) {
			if (this->IsMatched(*l, *r)) {
				this->_matched_obstacles.push_back(StereoObstacle(*l, *r));
				break;
			}
		}
	}

	return this->_matched_obstacles;
}

/// <summary>
/// 
/// </summary>
void ObstaclesCorrespondence::PrintMatchedObstacles(void) {
	for (auto o = this->_matched_obstacles.begin(); o != this->_matched_obstacles.end(); o++) {
		std::cout << (*o).GetLeftObstacle().ToString() << std::endl;
		std::cout << "corresponds to" << std::endl;
		std::cout << (*o).GetRightObstacle().ToString() << std::endl;
		std::cout << "Height: " << (*o).EvaluateHeight() << std::endl << std::endl;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="l"></param>
/// <param name="r"></param>
/// <returns></returns>
bool ObstaclesCorrespondence::IsMatched(Obstacle l, Obstacle r) {
	const int kMatchingPointsVariance = 15;
	const int kAreaVariance = 2000;
	bool res = true;
	int corner_matched_count = 0;
	std::vector<cv::Point> left_points = l.ToPoints();
	std::vector<cv::Point> right_points = r.ToPoints();
	
	// Search for similar points between the 2 obstacles
	for(auto lp = left_points.begin(); lp != left_points.end(); lp++) {
		for (auto rp = right_points.begin(); rp != right_points.end(); rp++) {
			if (std::abs(static_cast<int>((*lp).y - (*rp).y)) <= kMatchingPointsVariance) {
				corner_matched_count++;
				break;
			}
		}
	}

	// Matching conditions
	res &= (corner_matched_count == 4);
	res &= (std::abs(static_cast<int>(l.GetArea() - r.GetArea())) <= kAreaVariance);

	return res;
}
