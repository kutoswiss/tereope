#include "stdafx.h"
#include "CraneCamera.h"

/// <summary>
/// 
/// </summary>
CraneCamera::CraneCamera(VimbaSystem &vimbasystem, const char* pid) {
	if(vimbasystem.GetCameraByID(pid, this->_camera) != VmbErrorSuccess)
		std::cout << "ERROR: Unable to retrieve Vimba System cameras." << std::endl;
}

/// <summary>
/// 
/// </summary>
CraneCamera::~CraneCamera() {
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CameraPtr CraneCamera::GetCamera(void) {
	return this->_camera;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
FramePtr CraneCamera::GetFrame(void) {
	FramePtr frame;
	if(this->_camera->AcquireSingleImage(frame, this->kAcquireFrameTimeout) != VmbErrorSuccess)
		std::cout << "Unable to retrieve frame from camera." << std::endl;
	return frame;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
cv::Mat CraneCamera::GetMat(int type) {
	cv::Mat frame_mat = CameraHelper::FrameToCVMat(this->GetFrame(), type);
	return frame_mat;
}

/// <summary>
/// 
/// </summary>
void CraneCamera::Open(void) {
	std::string name;

	if (this->_camera->GetName(name) == VmbErrorSuccess)
		std::cout << name;

	if(this->_camera->Open(VmbAccessModeFull) == VmbErrorSuccess)
		std::cout << " camera opened" << std::endl;
}

/// <summary>
/// 
/// </summary>
void CraneCamera::Close(void) {
	std::string name;

	if (this->_camera->GetName(name) == VmbErrorSuccess)
		std::cout << name;

	if (this->_camera->Close() == VmbErrorSuccess)
		std::cout << " camera closed" << std::endl;
}
