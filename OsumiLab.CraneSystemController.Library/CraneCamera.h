#pragma once
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>
#include <string>

#define GUPPY_CAMERA_ID 0
#define PIKE_X_CAMERA_ID 1
#define PIKE_Y_CAMERA_ID 2

using namespace AVT::VmbAPI;

class CraneCamera
{
public:
	CraneCamera();
	CraneCamera( const char* pid, CameraPtr &camera);
	~CraneCamera();

	/// <summary>
	/// Public methods
	/// </summary>
	CameraPtrVector GetCameras(void);
	FramePtr GetFrame(void); 
	void Open(void);
	void Close(void);

protected:
	VimbaSystem &_vimba_system = VimbaSystem::GetInstance();
	//VimbaSystem *_vimba_system;
	CameraPtrVector _cameras;
};

