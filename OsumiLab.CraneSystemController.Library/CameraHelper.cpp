#include "stdafx.h"
#include "CameraHelper.h"

/// <summary>
/// 
/// </summary>
CameraHelper::CameraHelper()
{
}

/// <summary>
/// 
/// </summary>
CameraHelper::~CameraHelper()
{
}

/// <summary>
/// Static method to convert camera frame (AVT) into Mat (OpenCV)
/// </summary>
/// <param name="frame">Frame from AVT Camera</param>
/// <param name="type">Type of conversion</param>
/// <returns></returns>
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
