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
	this->SetAxisChannels(-1, -1, z_axis_channel);
}

/// <summary>
/// 
/// </summary>
CraneRopeAxis::~CraneRopeAxis() {
}


/// <summary>
/// Method to move the Axis 
/// </summary>
/// <param name="a">Axis to be moved</param>
/// <param name="step">Number of steps to move</param>
void CraneRopeAxis::Move(Axis a, int step, double voltage) {
	int channel = this->GetChannelFromAxis(a);

	short channel_start[8];
	unsigned long preset_data[8];
	for (int i = 0; i < 8; i++) {
		channel_start[i] = i;
		preset_data[i] = 2000000;
	}

	channel_start[0] = channel;
	AioSingleAoEx(this->_aio_id, channel, 1.0 * ((step < 0) ? -1 : 1));
	WaitUntilCounterReach(step, channel_start);
	AioSingleAoEx(this->_aio_id, channel, 0);
}