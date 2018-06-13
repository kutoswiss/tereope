#include "stdafx.h"
#include "CraneRopeAxis.h"

/// <summary>
/// 
/// </summary>
CraneRopeAxis::CraneRopeAxis() : CraneRopeAxis(-1, -1, -1) {
	// No code
}

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
/// <param name="z_axis_channel"></param>
CraneRopeAxis::CraneRopeAxis(short aio_id, short cnt_id, int z_axis_channel) {
	this->SetAioCntIDs(aio_id, cnt_id);
	this->SetAioChannels(-1, -1, z_axis_channel);
	this->_max_voltage = this->kMaxVoltage;
}

/// <summary>
/// 
/// </summary>
CraneRopeAxis::~CraneRopeAxis() {
}

/// <summary>
/// Convert meters to encoder steps value
/// </summary>
/// <param name="m">Meter unit</param>
/// <returns></returns>
double CraneRopeAxis::MeterToEncoderStep(double m) {
	return m * 54400 / 0.1; // 0.1[m] = 54'000 steps
}

/// <summary>
/// 
/// </summary>
void CraneRopeAxis::CalibratePresetValue() {
	short channel_start[8];
	unsigned long preset_data[8];
	short cnt = this->GetCntChannelFromAxis(Axis::Z);

	for (int i = 0; i < 8; i++) {
		channel_start[i] = i;
		preset_data[i] = CraneSettings::kCntPresetValue;
	}

	CntPreset(this->_cnt_id, &cnt, 1, preset_data);
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneRopeAxis::Move(Axis a, int step, double voltage) {
	short aio = this->GetAioChannelFromAxis(a);
	short cnt = this->GetCntChannelFromAxis(a);

	this->SetVoltage(voltage);
	this->Enable();
	AioSingleAoEx(this->_aio_id, aio, this->_voltage * ((step < 0) ? -1 : 1));
	this->WaitUntilCounterReach(step, &cnt);
	AioSingleAoEx(this->_aio_id, aio, 0);
	this->Disable();
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
void CraneRopeAxis::Stop(Axis a) {
	short aio = this->GetAioChannelFromAxis(a);
	AioSingleAoEx(this->_aio_id, aio, 0);
	this->Disable();
}


/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneRopeAxis::MoveTo(int step, double voltage) {
	short cnt = this->GetCntChannelFromAxis(Axis::Z);
	bool up = false;
	DWORD cntvalue;
	CntStartCount(this->_cnt_id, &cnt, 1);
	CntReadCount(this->_cnt_id, &cnt, 1, &cntvalue);
	cntvalue -= CraneSettings::kCntPresetValue; // Remove offset

	if (cntvalue < step) {
		up = true;
		step = step - cntvalue;
	} else if (cntvalue > step) {
		up = false;
		step = cntvalue - step;
	} else {
		step = 0;
	}

	if(step != 0) {
		this->Move(Axis::Z, step * ((up) ? 1 : -1), voltage);
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="meter"></param>
/// <param name="voltage"></param>
void CraneRopeAxis::Elevate(double meter, double voltage) {
	double m = CraneRopeAxis::MeterToEncoderStep(meter);
	this->Move(Axis::Z, m, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="meter"></param>
/// <param name="voltage"></param>
void CraneRopeAxis::ElevateTo(double meter, double voltage) {
	double m = CraneRopeAxis::MeterToEncoderStep(meter);
	this->MoveTo(m, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneRopeAxis::ToGround(double voltage) {
	this->MoveTo(1000, voltage);
	this->Move(Axis::Z, 500, 4);
	this->CalibratePresetValue();
}

/// <summary>
/// Enable coarse axis
/// </summary>
/// <param name="a"></param>
void CraneRopeAxis::Enable() {
	AioSingleAoEx(this->_aio_id, kZEnableChannel, 5.0);
}

/// <summary>
/// Disable coarse axis
/// </summary>
/// <param name="a"></param>
void CraneRopeAxis::Disable() {
	AioSingleAoEx(this->_aio_id, kZEnableChannel, 0.0);
}
