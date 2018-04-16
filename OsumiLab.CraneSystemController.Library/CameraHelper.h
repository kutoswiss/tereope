#pragma once
#include "opencv2\opencv.hpp"
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>

using namespace AVT::VmbAPI;

class CameraHelper
{
public:
	CameraHelper();
	~CameraHelper();
	static cv::Mat FrameToCVMat(FramePtr frame, int type);
};

