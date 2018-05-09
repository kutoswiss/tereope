#pragma once
#include <string>
#include <windows.h>
#include "Caio.h"
#include "CCnt.h"
#include "CraneCoarseAxis.h"
#include "CraneFineAxis.h"
#include "CraneRopeAxis.h"

#define AIO_DEVICE_NAME "AIO000"
#define CNT_DEVICE_NAME "CNT000"
#define X_COARSE_AXIS_CHANNEL 3
#define Y_COARSE_AXIS_CHANNEL 2
#define X_FINE_AXIS_CHANNEL 1
#define Y_FINE_AXIS_CHANNEL 0
#define Z_ROPE_AXIS_CHANNEL 4

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
	/// 
	/// </summary>
	/// <returns></returns>
	CraneCoarseAxis CoarseAxis() const;
	CraneFineAxis FineAxis() const;
	CraneRopeAxis Rope() const;

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

	/// <summary>
	/// Private members
	/// </summary>
	void InitAio(char *device_name);
	void InitCnt(char *device_name);
	void SetCoarseAxis(void);
	void SetFineAxis(void);
	void SetRopeAxis(void);
};

