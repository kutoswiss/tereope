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
	AioSingleAoEx(this->_aio_id, aio, this->_voltage * ((step < 0) ? -1 : 1));
	this->WaitUntilCounterReach(step, &cnt);
	AioSingleAoEx(this->_aio_id, aio, 0);
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneRopeAxis::MoveTo(int step, double voltage) {
	short aio = this->GetAioChannelFromAxis(Axis::Z);
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
		this->SetVoltage(voltage);
		AioSingleAoEx(this->_aio_id, aio, this->_voltage * (up) ? 1 : -1);
		this->WaitUntilCounterReach(step, &cnt);
		AioSingleAoEx(this->_aio_id, aio, 0);
	}
}