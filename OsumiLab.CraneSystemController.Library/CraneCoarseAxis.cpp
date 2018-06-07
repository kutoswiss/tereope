#include "stdafx.h"
#include "CraneCoarseAxis.h"

/// <summary>
/// 
/// </summary>
CraneCoarseAxis::CraneCoarseAxis() : CraneCoarseAxis(-1, -1, -1, -1) {
}

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneCoarseAxis::CraneCoarseAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel) {
	this->SetAioCntIDs(aio_id, cnt_id);
	this->SetAioChannels(x_axis_channel, y_axis_channel, -1);
}

/// <summary>
/// 
/// </summary>
CraneCoarseAxis::~CraneCoarseAxis() {
}

/// <summary>
/// Method to move the Axis 
/// </summary>
/// <param name="a">Axis to be moved</param>
/// <param name="step">Number of steps to move</param>
void CraneCoarseAxis::Move(Axis a, int step, double voltage) {
	short aio = this->GetAioChannelFromAxis(a);
	short cnt = this->GetCntChannelFromAxis(a);

	this->SetVoltage(voltage);
	AioSingleAoEx(this->_aio_id, aio, this->_voltage * ((step < 0) ? 1 : -1));
	this->WaitUntilCounterReach(step, &cnt);
	AioSingleAoEx(this->_aio_id, aio, 0);
}

