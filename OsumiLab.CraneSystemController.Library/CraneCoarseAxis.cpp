#include "stdafx.h"
#include "CraneCoarseAxis.h"

CraneCoarseAxis::CraneCoarseAxis() : CraneCoarseAxis(-1, -1) {
}

CraneCoarseAxis::CraneCoarseAxis(short aio_id, short cnt_id) {
	this->SetAioCntIDs(aio_id, cnt_id);
}

CraneCoarseAxis::~CraneCoarseAxis() {
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="step"></param>
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

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
int CraneCoarseAxis::GetChannelFromAxis(Axis a) {
	int channel = -1;

	switch (a) {
	case Axis::X:
		channel = this->kXAxisChannel;
		break;

	case Axis::Y:
		channel = this->kYAxisChannel;
		break;
	}
	
	return channel;
}
