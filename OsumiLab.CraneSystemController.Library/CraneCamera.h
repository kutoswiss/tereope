#pragma once
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>
#include <string>
#include "CraneSettings.h"

using namespace AVT::VmbAPI;

class CraneCamera
{
public:
	/// <summary>
	/// Ctor/Dtor
	/// </summary>
	CraneCamera(VimbaSystem &vimbasystem, const char* pid);
	~CraneCamera();

	/// <summary>
	/// Public methods
	/// </summary>
	CameraPtr GetCamera(void);
	FramePtr GetFrame(void); 
	void Open(void);
	void Close(void);

protected:
	/// <summary>
	/// Constants
	/// </summary>
	const int kAcquireFrameTimeout = 500;

	/// <summary>
	/// Members
	/// </summary>
	CameraPtr _camera;
};

