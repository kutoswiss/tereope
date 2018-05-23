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
	this->SetAxisChannels(x_axis_channel, y_axis_channel, -1);
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
void CraneCoarseAxis::Move(Axis a, int step) {
	int channel = this->GetChannelFromAxis(a);
	step *= -1;

	short channel_start[8];
	for (int i = 0; i < 8; i++)
		channel_start[i] = i;

	channel_start[0] = channel;
	AioSingleAoEx(this->_aio_id, channel, 0.2 * ((step < 0) ? -1 : 1));
	WaitUntilCounterReach(step, channel_start);
	AioSingleAoEx(this->_aio_id, channel, 0);
}

