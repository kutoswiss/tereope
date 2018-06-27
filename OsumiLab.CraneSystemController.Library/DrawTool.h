#pragma once
#include "opencv2\opencv.hpp"

class DrawTool
{
public:
	// Ctor/Dtor
	DrawTool();
	~DrawTool();

	// Public methods
	void SetFrame(cv::Mat &frame);
	cv::Mat GetFrame();
	
protected:
	// Private virtual methods
	virtual void Draw() = 0;

	// Private members
	cv::Mat _frame;
};

