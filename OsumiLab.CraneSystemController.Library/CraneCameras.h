#pragma once
#include <VimbaCPP.h>
#include <VimbaSystem.hpp>
#include <iostream>
#include <string>

using namespace AVT::VmbAPI;

class CraneCameras
{
public:
	/// <summary>
	/// Ctor / Dtor
	/// </summary>
	CraneCameras();
	~CraneCameras();

	/// <summary>
	/// Public methods
	/// </summary>
	/// <returns></returns>
	CameraPtrVector GetCameras();
	FramePtr GetFrame(int cam_index);

private:
	/// <summary>
	/// Private constants
	/// </summary>
	const int kAcquireFrameTimeout = 500;

	/// <summary>
	/// Private methods
	/// </summary>
	void OpenAllCameras();
	void CloseAllCameras();

	/// <summary>
	/// Private fields
	/// </summary>
	VimbaSystem &_vimba_system = VimbaSystem::GetInstance();
	CameraPtrVector _cameras;
};

