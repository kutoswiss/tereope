#pragma once
#include "CraneCamera.h"

class CraneRopeCamera : public CraneCamera
{
public:
	/// <summary>
	/// Ctors/Dtors
	/// </summary>
	CraneRopeCamera(VimbaSystem &vimbasystem);
	CraneRopeCamera(VimbaSystem &vimbasystem, const char *pid);
	~CraneRopeCamera();
};

typedef std::unique_ptr<CraneRopeCamera> CraneRopeCameraPtr;
