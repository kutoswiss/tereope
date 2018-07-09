#pragma once
#include "CraneMovementSystem.h"
#include "CraneSettings.h"

class CraneRopeMS : public CraneMovementSystem
{
public:
	// Ctor/Dtor
	CraneRopeMS(short aio_id, short cnt_id);
	~CraneRopeMS();

	// Static methods
	static double MeterToEncoderStep(double m);

	// Public methods
	void CalibratePresetValue();
	void MoveTo(int step, double voltage);
	void Elevate(double meter, double voltage);
	void ElevateTo(double meter, double voltage);
	void ToGround(double voltage);

protected:
	void Move(char *axis_key, int step, double voltage = 0);

private:
	// Private constants
	char* kZAxisKey = "Z";
	const int kZAxisAioChannel = CraneSettings::kZRopeAxisAioChannel;
	const int kZAxisCntChannel = CraneSettings::kZRopeAxisCntChannel;
	const int kZAxisEnableChannel = 11;
};