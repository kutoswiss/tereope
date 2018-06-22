#pragma once
#include "AbstractCraneAxis.h"

class CraneFineAxis : public AbstractCraneAxis
{
public:
	// Constants
	const int kXEnableChannel = 8;
	const int kYEnableChannel = 7;

	// Ctor/Dtor
	CraneFineAxis();
	CraneFineAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel);
	~CraneFineAxis();

	// Virtual Implemented methods
	void Move(Axis a, int step, double voltage = 0);
	void Stop(Axis a);

	// Public methods
	void MoveX(int step, double voltage);
	void MoveY(int step, double voltage);

	void SetAxisVoltage(Axis a, double voltage);
private:
	// Private methods
	void Enable(Axis a);
	void Disable(Axis a);
	void SetEnableVoltage(Axis a, const double voltage);
};

