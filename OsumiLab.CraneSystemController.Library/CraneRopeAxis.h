#pragma once
#include "AbstractCraneAxis.h"

class CraneRopeAxis : public AbstractCraneAxis
{
public:
	// Ctor/Dtor
	CraneRopeAxis();
	CraneRopeAxis(short aio_id, short cnt_id, int z_axis_channel);
	~CraneRopeAxis();

	// Constants
	static constexpr double kMaxVoltage = 4.0;

	// Static methods
	static double MeterToEncoderStep(double m);

	// Public methods
	void CalibratePresetValue();
	void MoveTo(int step, double voltage);
	void Elevate(double meter, double voltage = kMaxVoltage);
	void ElevateTo(double meter, double voltage = kMaxVoltage);
	void ToGround(double voltage = kMaxVoltage);

	// Implemented methods
	void Move(Axis a, int step, double voltage = 0);
};

