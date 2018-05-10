#include "stdafx.h"
#include "CraneFineAxis.h"

/// <summary>
/// 
/// </summary>
CraneFineAxis::CraneFineAxis() : CraneFineAxis(-1, -1, -1, -1) {
}

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneFineAxis::CraneFineAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel) {
	this->SetAioCntIDs(aio_id, cnt_id);
	this->SetAxisChannels(x_axis_channel, y_axis_channel, -1);
}

/// <summary>
/// 
/// </summary>
CraneFineAxis::~CraneFineAxis() {
}

/// <summary>
/// Method to move the Axis 
/// </summary>
/// <param name="a">Axis to be moved</param>
/// <param name="step">Number of steps to move</param>
void CraneFineAxis::Move(Axis a, int step) {
	int channel = this->GetChannelFromAxis(a);
	step *= -1;

	short channel_start[8];
	unsigned long preset_data[8];
	for (int i = 0; i < 8; i++) {
		channel_start[i] = i;
		preset_data[i] = 2000000;
	}

	DWORD current_value = 0;
	DWORD initial_value = 0;
	channel_start[0] = 0;
	
	CntStartCount(this->_cnt_id, channel_start, 1);
	CntReadCount(this->_cnt_id, channel_start, 1, &initial_value);
	AioSingleAoEx(this->_aio_id, channel, 0.5 * ((step < 0) ? -1 : 1));
	//WaitUntilCounterReach(step, channel_start);
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	AioSingleAoEx(this->_aio_id, channel, 0);
	CntReadCount(this->_cnt_id, channel_start, 1, &current_value);
	CntStopCount(this->_cnt_id, channel_start, 1);
}
