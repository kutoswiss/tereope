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