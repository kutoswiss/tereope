#pragma once
#include "CraneCamera.h"

class CraneRopeCamera : public CraneCamera
{
public:
	CraneRopeCamera(VimbaSystem &vimbasystem);
	CraneRopeCamera(VimbaSystem &vimbasystem, const char *pid);
	~CraneRopeCamera();
};

