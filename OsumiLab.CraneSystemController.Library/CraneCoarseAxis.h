#pragma once
#include "AbstractCraneAxis.h"

class CraneCoarseAxis : public AbstractCraneAxis
{
public:
	/// <summary>
	/// Constants
	/// </summary>
	const static int kXAxisChannel = 3;
	const static int kYAxisChannel = 2;

	/// <summary>
	/// Ctor/Dtor
	/// </summary>
	CraneCoarseAxis();
	CraneCoarseAxis(short aio_id, short cnt_id);
	~CraneCoarseAxis();

	/// <summary>
	/// Virtal methods implemented
	/// </summary>
	/// <param name="a"></param>
	/// <param name="step"></param>
	void Move(Axis a, int step);

private:
	/// <summary>
	/// Private functions
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	int GetChannelFromAxis(Axis a);
};

