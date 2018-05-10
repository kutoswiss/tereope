#include "stdafx.h"
#include "CraneCamera.h"

CraneCamera::CraneCamera() {
}

/// <summary>
/// 
/// </summary>
CraneCamera::CraneCamera(const char* pid, CameraPtr &camera) {
	if(this->_vimba_system.Startup() != VmbErrorSuccess)
		std::cout << "ERROR: Unable to startup Vimba System" << std::endl;

	if(this->_vimba_system.GetCameraByID(pid, camera) != VmbErrorSuccess)
		std::cout << "ERROR: Unable to retrieve Vimba System cameras." << std::endl;
}

/// <summary>
/// 
/// </summary>
CraneCamera::~CraneCamera() {
	if (this->_vimba_system.Shutdown() != VmbErrorSuccess)
		std::cout << "ERROR: Unable to shutdown Vimba System" << std::endl;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
CameraPtrVector CraneCamera::GetCameras(void) {
	return this->_cameras;
}
