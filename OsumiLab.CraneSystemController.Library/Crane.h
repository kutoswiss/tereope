#pragma once
#include <string>
#include <windows.h>
#include "Caio.h"
#include "CCnt.h"
#include "CraneCoarseAxis.h"
#include "CraneFineAxis.h"
#include "CraneRopeAxis.h"
#include "CraneSceneCamera.h"
#include "CraneRopeCamera.h"
#include "CraneSettings.h"

class Crane
{
public:
	/// <summary>
	/// Constants
	/// </summary>
	static const unsigned int kAioMaxChannel = 16;
	static const unsigned int kCntMaxChannel = 8;

	/// <summary>
	/// Ctor/Dtor
	/// </summary>
	Crane();
	Crane(char *aio_name, char *cnt_name);
	~Crane();

	/// <summary>
	/// Getters
	/// </summary>
	/// <returns></returns>
	CraneCoarseAxis CoarseAxis() const;
	CraneFineAxis FineAxis() const;
	CraneRopeAxis Rope() const;
	CraneSceneCamera& RightSceneCamera() const;
	CraneRopeCamera& XRopeCamera() const;
	CraneRopeCamera& YRopeCamera() const;

	/// <summary>
	/// Public methods
	/// </summary>
	void Exit();

private:
	/// <summary>
	///  private members
	/// </summary>
	short _aio_id;
	short _cnt_id;
	float _aio_data[kAioMaxChannel];
	CraneCoarseAxis _coarse_axis;
	CraneFineAxis _fine_axis;
	CraneRopeAxis _rope;
	VimbaSystem &_vimbasystem = VimbaSystem::GetInstance();
	//CraneSceneCameraPtr _cam_scene_right;
	CraneSceneCamera *_cam_scene_right;

	CraneRopeCameraPtr _cam_rope_x;
	CraneRopeCameraPtr _cam_rope_y;

	/// <summary>
	/// Private members
	/// </summary>
	void InitAio(char *device_name);
	void InitCnt(char *device_name);
	void InitCameras(void);
	void SetCoarseAxis(void);
	void SetFineAxis(void);
	void SetRopeAxis(void);
	void VimbaSystemStartup(void);
	void VimbaSystemShutdown(void);
};

