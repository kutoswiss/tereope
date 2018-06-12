#pragma once
#include "AbstractCraneAxis.h"

class CraneFineAxis : public AbstractCraneAxis
{
public:
	// Ctor/Dtor
	CraneFineAxis();
	CraneFineAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel);
	~CraneFineAxis();

	// Virtual Implemented methods
	void Move(Axis a, int step, double voltage = 0);
	void Stop(Axis a);
};

