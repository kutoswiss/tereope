#include "stdafx.h"
#include "CraneCameras.h"

/// <summary>
/// Constructor
/// </summary>
CraneCameras::CraneCameras()
{
	if (this->vimba_system_.Startup() != VmbErrorSuccess)
		std::cout << "ERROR: Unable to startup Vimba System" << std::endl;

	if (this->vimba_system_.GetCameras(this->cameras_) != VmbErrorSuccess)
		std::cout << "ERROR: Unable to retrieve Vimba System cameras." << std::endl;

	this->OpenAllCameras();
}

/// <summary>
/// Destructor
/// </summary>
CraneCameras::~CraneCameras()
{
	this->CloseAllCameras();

	if (this->vimba_system_.Shutdown() != VmbErrorSuccess)
		std::cout << "ERROR: Unable to shutdown Vimba System" << std::endl;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CameraPtrVector CraneCameras::GetCameras() {
	return this->cameras_;
}

/// <summary>
/// 
/// </summary>
/// <param name="cam_index"></param>
/// <returns></returns>
FramePtr CraneCameras::GetFrame(int cam_index) {
	FramePtr frame;
	if (this->cameras_[cam_index]->AcquireSingleImage(frame, this->kAcquireFrameTimeout) != VmbErrorSuccess)
		std::cout << "Unable to retrieve frame from camera." << std::endl;

	return frame;
}

/// <summary>
/// 
/// </summary>
void CraneCameras::OpenAllCameras() {
	std::string name;
	for (auto cam = this->cameras_.begin(); cam != this->cameras_.end(); cam++) {
		if ((*cam)->GetName(name) == VmbErrorSuccess)
			std::cout << name;

		if ((*cam)->Open(VmbAccessModeFull) == VmbErrorSuccess)
			std::cout << " camera opened" << std::endl;
	}
}

/// <summary>
/// 
/// </summary>
void CraneCameras::CloseAllCameras() {
	std::string name;
	for (auto cam = this->cameras_.begin(); cam != this->cameras_.end(); cam++) {
		if ((*cam)->GetName(name) == VmbErrorSuccess)
			std::cout << name;

		if ((*cam)->Close() == VmbErrorSuccess)
			std::cout << " camera closed" << std::endl;
	}
}