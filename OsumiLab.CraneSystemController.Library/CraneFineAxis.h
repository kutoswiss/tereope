#pragma once
#include "AbstractCraneAxis.h"

class CraneFineAxis : public AbstractCraneAxis
{
public:
	CraneFineAxis();
	CraneFineAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel);
	~CraneFineAxis();

	/// <summary>
	/// Implemented methods
	/// </summary>
	void Move(Axis a, int step, double voltage = 0);
};

