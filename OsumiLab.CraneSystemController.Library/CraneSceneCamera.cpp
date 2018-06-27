#include "stdafx.h"
#include "CraneSceneCamera.h"

/// <summary>
/// 
/// </summary>
/// <param name="vimbasystem"></param>
CraneSceneCamera::CraneSceneCamera(VimbaSystem &vimbasystem) 
	: CraneSceneCamera(vimbasystem, CraneSettings::kGuppyRightCameraPID) {
	// No code
}

/// <summary>
/// 
/// </summary>
/// <param name="vimbasystem"></param>
/// <param name="pid"></param>
CraneSceneCamera::CraneSceneCamera(VimbaSystem &vimbasystem, const char *pid)
	: CraneCamera(vimbasystem, pid) {
	this->Open();

}

/// <summary>
/// 
/// </summary>
CraneSceneCamera::~CraneSceneCamera() {
	this->Close();
}