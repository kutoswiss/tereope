#pragma once
#include <windows.h>
#include <thread>
#include <chrono>
#include <future>
#include "Caio.h"
#include "CCnt.h"
#include "CraneSettings.h"

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
	// Virtual methods
	virtual ~AbstractCraneAxis() {};
	virtual void Move(Axis a, int step, double voltage = 0) = 0;
	virtual void Stop(Axis a) = 0;

	// Setters
	void SetAioChannels(int x, int y, int z);
	void SetCntChannels(int x, int y, int z);
	void SetAioCntIDs(short aio, short cnt);
	void SetAioID(short id);
	void SetCntID(short id);
	void SetVoltage(double voltage);
	int GetCntValue(Axis a);

protected:
	// Protected methods
	void WaitUntilCounterReach(int step, short *channels);
	void WaitUntilCounterReach(int step, short *channels, bool *stop_signal);
	int GetAioChannelFromAxis(Axis axis);
	int GetCntChannelFromAxis(Axis axis);

	// Protected members
	short _aio_id;
	short _cnt_id;
	double _voltage = 0;
	double _min_voltage = 0;
	double _max_voltage = 2.0;

private:
	// Private members
	int _axis_aio_channels[3];
	int _axis_cnt_channels[3];
	DWORD _current_value = 0;
	DWORD _initial_value = 0;
};
