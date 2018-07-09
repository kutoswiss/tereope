#pragma once
#include "CraneMovementSystem.h"
#include "CraneSettings.h"

class CraneFineMS : public CraneMovementSystem
{
public:
	CraneFineMS(short aio_id, short cnt_id);
	~CraneFineMS();

	// Public method
	void X(double voltage);
	void Y(double voltage);
	void HaltX();
	void HaltY();

protected:
	void Move(char *axis_key, int step, double voltage = 0);

private:
	// Private constants
	char* kXAxisKey = "X";
	char* kYAxisKey = "Y";
	const int kXAxisAioChannel = CraneSettings::kXFineAxisAioChannel;
	const int kYAxisAioChannel = CraneSettings::kYFineAxisAioChannel;
	const int kXAxisCntChannel = CraneSettings::kXFineAxisCntChannel;
	const int kYAxisCntChannel = CraneSettings::kYFineAxisCntChannel;
	const int kXAxisEnableChannel = 8;
	const int kYAxisEnableChannel = 7;
};

