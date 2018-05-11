#include "stdafx.h"
#include "CraneRopeCamera.h"


/// <summary>
/// 
/// </summary>
/// <param name="vimbasystem"></param>
CraneRopeCamera::CraneRopeCamera(VimbaSystem &vimbasystem) 
	: CraneRopeCamera(vimbasystem, CraneSettings::kPikeXCameraPID) {
	// No code
}

/// <summary>
/// 
/// </summary>
/// <param name="vimbasystem"></param>
/// <param name="pid"></param>
CraneRopeCamera::CraneRopeCamera(VimbaSystem &vimbasystem, const char *pid) 
	: CraneCamera(vimbasystem, pid) {
	this->Open();
}

/// <summary>
/// 
/// </summary>
CraneRopeCamera::~CraneRopeCamera() {
	this->Close();
}
