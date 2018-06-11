#pragma once
#include "AbstractCraneAxis.h"

class CraneCoarseAxis : public AbstractCraneAxis
{
public:
	// Constants
	const int kDefaultVoltage = 0.2;

	// Ctor/Dtor
	CraneCoarseAxis();
	CraneCoarseAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel);
	~CraneCoarseAxis();

	// Virtal methods implemented
	void Move(Axis a, int step, double voltage = 0);
	void Stop(Axis a);

	// Public methods
	void MoveXThread(int& step, double& voltage);

private:
	std::unique_ptr<std::thread> _x_axis_thread;
	//std::unique_ptr<std::thread> _y_axis_thread;
};

