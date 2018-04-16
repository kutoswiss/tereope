#include "stdafx.h"
#include "Obstacle.h"


Obstacle::Obstacle() {
}


Obstacle::~Obstacle() {
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
unsigned int Obstacle::GetDepth() const {
	return this->depth_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
unsigned int Obstacle::GetHeight() const {
	return this->height_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
unsigned int Obstacle::GetWidth() const {
	return this->width_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
unsigned int Obstacle::GetXOrigin() const {
	return this->x_origin_;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
unsigned int Obstacle::GetYOrigin() const {
	return this->y_origin_;
}
