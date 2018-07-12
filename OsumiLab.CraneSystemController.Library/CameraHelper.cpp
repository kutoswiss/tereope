#include "stdafx.h"
#include "CameraHelper.h"

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

	cv::Mat m = cv::Mat(height, width, type, image);
	cv::cvtColor(m, m, CV_BayerBG2RGB);

	return m;
}

/// <summary>
/// 
/// </summary>
/// <param name="size"></param>
/// <returns></returns>
cv::Mat CameraHelper::GetOnesKernel(int size) {
	return cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size));
}
