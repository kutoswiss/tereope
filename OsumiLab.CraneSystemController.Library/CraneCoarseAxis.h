#pragma once
#include "AbstractCraneAxis.h"

class CraneCoarseAxis : public AbstractCraneAxis
{
public:
	/// <summary>
	/// Ctor/Dtor
	/// </summary>
	CraneCoarseAxis();
	CraneCoarseAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel);
	~CraneCoarseAxis();

	/// <summary>
	/// Virtal methods implemented
	/// </summary>
	void Move(Axis a, int step);
};

