#include "stdafx.h"
#include "AbstractCraneAxis.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio"></param>
/// <param name="cnt"></param>
void AbstractCraneAxis::SetAioCntIDs(short aio, short cnt) {
	this->SetAioID(aio);
	this->SetCntID(cnt);
}

/// <summary>
/// 
/// </summary>
/// <param name="id"></param>
void AbstractCraneAxis::SetAioID(short id) {
	this->_aio_id = id;
}

/// <summary>
/// 
/// </summary>
/// <param name="id"></param>
void AbstractCraneAxis::SetCntID(short id) {
	this->_cnt_id = id;
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="channels"></param>
void AbstractCraneAxis::WaitUntilCounterReach(int step, short *channels) {
	DWORD current_value = 0;
	DWORD initial_value = 0;

	CntStartCount(this->_cnt_id, channels, 1);
	CntReadCount(this->_cnt_id, channels, 1, &initial_value);
	CntReadCount(this->_cnt_id, channels, 1, &current_value);
	
	// Wait until the expected value is reached
	while (std::abs(static_cast<long>(current_value - initial_value)) < std::abs(step))
		CntReadCount(this->_cnt_id, channels, 1, &current_value);

	CntStopCount(this->_cnt_id, channels, 1);
}