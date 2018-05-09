#pragma once
#include "AbstractCraneAxis.h"

class CraneRopeAxis : public AbstractCraneAxis
{
public:
	CraneRopeAxis();
	CraneRopeAxis(short aio_id, short cnt_id, int z_axis_channel);
	~CraneRopeAxis();

	/// <summary>
	/// Implemented methods
	/// </summary>
	void Move(Axis a, int step);
};

