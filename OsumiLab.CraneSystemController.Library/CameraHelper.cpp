#include "stdafx.h"
#include "CameraHelper.h"


CameraHelper::CameraHelper()
{
}


CameraHelper::~CameraHelper()
{
}

cv::Mat CameraHelper::FrameToCVMat(FramePtr frame, int type) {
	VmbUchar_t *image = NULL;
	VmbUint32_t width = 0;
	VmbUint32_t height = 0;

	if (VmbErrorSuccess != frame->GetWidth(width))
		std::cout << "FAILED to aquire width of frame!" << std::endl;

	if (VmbErrorSuccess != frame->GetHeight(height))
		std::cout << "FAILED to aquire height of frame!" << std::endl;

	if (VmbErrorSuccess != frame->GetImage(image))
		std::cout << "FAILED to acquire image data of frame!" << std::endl;

	return cv::Mat(height, width, type, image);
}
