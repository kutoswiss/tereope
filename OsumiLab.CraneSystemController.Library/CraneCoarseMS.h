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
	void X(double voltage);
	void Y(double voltage);
	void X(int step, double voltage);
	void Y(int step, double voltage);
	void XThread(int step, double voltage);
	void YThread(int step, double voltage);
	void HaltX();
	void HaltY();
	int GetXCntValue();
	int GetYCntValue();

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
	void XJoinThread();
	void YJoinThread();
};

