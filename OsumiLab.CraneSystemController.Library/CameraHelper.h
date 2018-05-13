#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>

using namespace AVT::VmbAPI;

class CameraHelper
{
public:
	static cv::Mat FrameToCVMat(FramePtr frame, int type);
};

