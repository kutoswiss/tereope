#pragma once
#include <windows.h>
#include <thread>
#include <chrono>
#include "Caio.h"
#include "CCnt.h"

using namespace std::chrono;

/// <summary>
/// Enum
/// </summary>
typedef enum Axis {
	X = 0,
	Y = 1,
	Z = 2
};

class AbstractCraneAxis {
public:
	/// <summary>
	/// Virtual methods
	/// </summary>
	virtual ~AbstractCraneAxis() {};
	virtual void Move(Axis a, int step, double voltage = 0) = 0;
	//virtual void MoveTo(Axis a, int position) = 0;

	/// <summary>
	/// Setters
	/// </summary>
	/// <param name="aio"></param>
	/// <param name="cnt"></param>
	void SetAioChannels(int x, int y, int z);
	void SetCntChannels(int x, int y, int z);
	void SetAioCntIDs(short aio, short cnt);
	void SetAioID(short id);
	void SetCntID(short id);
	void SetVoltage(double voltage);

	bool IsMoving() const;

protected:
	/// <summary>
	/// Protected methods
	/// </summary>
	/// <param name="step"></param>
	/// <param name="channels"></param>
	void WaitUntilCounterReach(int step, short *channels);
	int GetAioChannelFromAxis(Axis axis);
	int GetCntChannelFromAxis(Axis axis);

	/// <summary>
	/// Protected members
	/// </summary>
	short _aio_id;
	short _cnt_id;
	bool _is_moving = false;
	double _voltage = 0;
	double _min_voltage = 0;
	double _max_voltage = 2.0;

private:
	/// <summary>
	/// Private members
	/// </summary>
	int _axis_aio_channels[3];
	int _axis_cnt_channels[3];
};
