#include "stdafx.h"
#include "AbstractCraneAxis.h"

/// <summary>
/// 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void AbstractCraneAxis::SetAxisChannels(int x, int y, int z) {
	this->_axis_channels[Axis::X] = x;
	this->_axis_channels[Axis::Y] = y;
	this->_axis_channels[Axis::Z] = z;
}

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
/// <param name="voltage"></param>
void AbstractCraneAxis::SetVoltage(double voltage) {
	if (voltage <= 0)
		this->_voltage = this->_min_voltage;
	else if (voltage > this->_max_voltage)
		this->_voltage = this->_max_voltage;
	else
		this->_voltage = voltage;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
bool AbstractCraneAxis::IsMoving() const {
	return this->_is_moving;
}


/// <summary>
/// Method that makes an active wait until the counter value is reached
/// </summary>
/// <param name="step"></param>
/// <param name="channels"></param>
void AbstractCraneAxis::WaitUntilCounterReach(int step, short *channels) {
	this->_is_moving = true;
	DWORD current_value = 0;
	DWORD initial_value = 0;

	CntStartCount(this->_cnt_id, channels, 1);
	CntReadCount(this->_cnt_id, channels, 1, &initial_value);
	CntReadCount(this->_cnt_id, channels, 1, &current_value);

	// Wait until the expected value is reached
	while (std::abs(static_cast<long>(current_value - initial_value)) < std::abs(step)) {
		CntReadCount(this->_cnt_id, channels, 1, &current_value);
	}

	CntStopCount(this->_cnt_id, channels, 1);
	this->_is_moving = false;
}

/// <summary>
/// 
/// </summary>
/// <param name="axis"></param>
/// <returns></returns>
int AbstractCraneAxis::GetChannelFromAxis(Axis axis) {
	return this->_axis_channels[axis];
}