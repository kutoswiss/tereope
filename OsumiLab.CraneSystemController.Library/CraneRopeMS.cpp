#include "stdafx.h"
#include "CraneRopeMS.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneRopeMS::CraneRopeMS(short aio_id, short cnt_id) {
	_aio_id = aio_id;
	_cnt_id = cnt_id;

	// Setup the Z axis
	this->SetAxis(this->kZAxisKey,
		std::make_shared<CraneAxis>(_aio_id, _cnt_id,
			this->kZAxisAioChannel,
			this->kZAxisCntChannel,
			this->kZAxisEnableChannel));

	// Modify the max voltage of the axis
	_axis[this->kZAxisKey]->SetMaxVoltage(kMaxVoltage);
}

/// <summary>
/// 
/// </summary>
CraneRopeMS::~CraneRopeMS() {
}

/// <summary>
/// Convert meters to encoder steps value
/// </summary>
/// <param name="m">Meter unit</param>
/// <returns></returns>
double CraneRopeMS::MeterToEncoderStep(double m) {
	return m * 54400 / 0.1; // 0.1[m] = 54'000 steps
}

/// <summary>
/// 
/// </summary>
void CraneRopeMS::CalibratePresetValue() {
	short channel_start[8];
	unsigned long preset_data[8];
	short cnt = this->kZAxisCntChannel;

	for (int i = 0; i < 8; i++) {
		channel_start[i] = i;
		preset_data[i] = CraneSettings::kCntPresetValue;
	}

	CntPreset(this->_cnt_id, &cnt, 1, preset_data);
}

/// <summary>
/// 
/// </summary>
/// <param name="axis_key"></param>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneRopeMS::Move(char *axis_key, int step, double voltage) {
	voltage = voltage * ((step < 0) ? -1 : 1);

	_axis[axis_key]->Enable();
	_axis[axis_key]->SetVoltage(voltage);
	_axis[axis_key]->WaitUntilCounterReach(step);
	_axis[axis_key]->SetVoltage(0.0);
	_axis[axis_key]->Disable();
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneRopeMS::MoveTo(int step, double voltage) {
	short cnt = this->kZAxisCntChannel;
	bool up = false;
	DWORD cntvalue;
	CntStartCount(this->_cnt_id, &cnt, 1);
	CntReadCount(this->_cnt_id, &cnt, 1, &cntvalue);
	cntvalue -= CraneSettings::kCntPresetValue; // Remove offset

	if (cntvalue < step) {
		up = true;
		step = step - cntvalue;
	}
	else if (cntvalue > step) {
		up = false;
		step = cntvalue - step;
	}
	else {
		step = 0;
	}

	if (step != 0) {
		this->Move(this->kZAxisKey, step * ((up) ? 1 : -1), voltage);
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="meter"></param>
/// <param name="voltage"></param>
void CraneRopeMS::Elevate(double meter, double voltage) {
	double m = CraneRopeMS::MeterToEncoderStep(meter);
	this->Move(this->kZAxisKey, m, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="meter"></param>
/// <param name="voltage"></param>
void CraneRopeMS::ElevateTo(double meter, double voltage) {
	double m = CraneRopeMS::MeterToEncoderStep(meter);
	this->MoveTo(m, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneRopeMS::ToGround(double voltage) {
	this->MoveTo(1000, voltage);
	this->Move(this->kZAxisKey, 500, 4);
	this->CalibratePresetValue();
}
