#pragma once
#include "AbstractCraneAxis.h"

class CraneCoarseAxis : public AbstractCraneAxis
{
public:
	// Constants
	const int kDefaultVoltage = 0.2;
	const int kXEnableChannel = 10;
	const int kYEnableChannel = 9;

	// Ctor/Dtor
	CraneCoarseAxis();
	CraneCoarseAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel);
	~CraneCoarseAxis();

	// Virtal methods implemented
	void Move(Axis a, int step, double voltage = 0);
	void Stop(Axis a);

	// Public methods
	void MoveX(int step, double voltage);
	void MoveY(int step, double voltage);
	
private:
	// Private methods
	void Enable(Axis a);
	void Disable(Axis a);
	void SetEnableVoltage(Axis a, const double voltage);
	void MoveXThread(int& step, double& voltage);
	void MoveYThread(int& step, double& voltage);

	// Private members
	std::unique_ptr<std::thread> _x_axis_thread;
	std::unique_ptr<std::thread> _y_axis_thread;
	bool _x_stop_signal = false;
	bool _y_stop_signal = false;
};

