#pragma once
#include "AbstractCraneAxis.h"

class CraneRopeAxis : public AbstractCraneAxis
{
public:
	// Constants
	const double kMaxVoltage = 4.0;

	// Ctor/Dtor
	CraneRopeAxis();
	CraneRopeAxis(short aio_id, short cnt_id, int z_axis_channel);
	~CraneRopeAxis();

	// Implemented methods
	void Move(Axis a, int step, double voltage = 0);
};

