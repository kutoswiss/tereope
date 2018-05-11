#pragma once
#include "CraneCamera.h"

class CraneSceneCamera : public CraneCamera
{
public:
	CraneSceneCamera(VimbaSystem &vimbasystem);
	CraneSceneCamera(VimbaSystem &vimbasystem, const char *pid);
	~CraneSceneCamera();
};

