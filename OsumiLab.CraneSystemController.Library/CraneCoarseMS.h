#pragma once
#include "CraneMovementSystem.h"
#include "CraneSettings.h"
#include <thread>

class CraneCoarseMS : public CraneMovementSystem
{
public:
	// Ctor/Dtor
	CraneCoarseMS(short aio_id, short cnt_id);
	~CraneCoarseMS();

	// Public methods
	void X(int step, double voltage);
	void Y(int step, double voltage);
	void XThread(int step, double voltage);
	void YThread(int step, double voltage);
	void HaltX();
	void HaltY();

protected:
	void Move(char *axis_key, int step, double voltage = 0);

private:
	// Private constants
	char* kXAxisKey = "X";
	char* kYAxisKey = "Y";
	const int kXAxisAioChannel = CraneSettings::kXCoarseAxisAioChannel;
	const int kYAxisAioChannel = CraneSettings::kYCoarseAxisAioChannel;
	const int kXAxisCntChannel = CraneSettings::kXCoarseAxisCntChannel;
	const int kYAxisCntChannel = CraneSettings::kYCoarseAxisCntChannel;
	const int kXAxisEnableChannel = 10;
	const int kYAxisEnableChannel = 9;

	// Private methods
	void XThreadImpl(int &step, double &voltage);
	void YThreadImpl(int &step, double &voltage);
	void XJoinThread();
	void YJoinThread();

	// Private members
	std::unique_ptr<std::thread> _x_thread;
	std::unique_ptr<std::thread> _y_thread;
};

