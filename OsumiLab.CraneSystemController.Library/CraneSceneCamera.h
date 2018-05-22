#pragma once
#include "CraneCamera.h"

class CraneSceneCamera : public CraneCamera
{
public:
	/// <summary>
	/// Ctors/Dtors
	/// </summary>
	CraneSceneCamera(VimbaSystem &vimbasystem);
	CraneSceneCamera(VimbaSystem &vimbasystem, const char *pid);
	~CraneSceneCamera();
};

typedef std::unique_ptr<CraneSceneCamera> CraneSceneCameraPtr;