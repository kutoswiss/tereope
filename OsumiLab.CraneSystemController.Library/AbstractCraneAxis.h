#pragma once
#include <windows.h>
#include <thread>
#include "Caio.h"
#include "CCnt.h"

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
	virtual void Move(Axis a, int step) = 0;
	//virtual void MoveTo(Axis a, int position) = 0;

	/// <summary>
	/// Setters
	/// </summary>
	/// <param name="aio"></param>
	/// <param name="cnt"></param>
	void SetAxisChannels(int x, int y, int z);
	void SetAioCntIDs(short aio, short cnt);
	void SetAioID(short id);
	void SetCntID(short id);

protected:
	/// <summary>
	/// Protected methods
	/// </summary>
	/// <param name="step"></param>
	/// <param name="channels"></param>
	void WaitUntilCounterReach(int step, short *channels);
	int GetChannelFromAxis(Axis axis);

	/// <summary>
	/// Protected members
	/// </summary>
	short _aio_id;
	short _cnt_id;

private:
	/// <summary>
	/// Private members
	/// </summary>
	int _axis_channels[3];
};
