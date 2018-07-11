#pragma once

#include "CameraHelper.h"
#include "CraneSettings.h"

#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>
#include <string>
#include <memory>


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
	void GetFrame(FramePtr frame);
	FramePtrVector GetFrames(void);

	cv::Mat GetMat(int type);
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

