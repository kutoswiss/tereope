#include "stdafx.h"
#include "AbstractCraneAxis.h"

/// <summary>
/// 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void AbstractCraneAxis::SetAioChannels(int x, int y, int z) {
	this->_axis_aio_channels[Axis::X] = x;
	this->_axis_aio_channels[Axis::Y] = y;
	this->_axis_aio_channels[Axis::Z] = z;
}

/// <summary>
/// 
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
void AbstractCraneAxis::SetCntChannels(int x, int y, int z) {
	this->_axis_cnt_channels[Axis::X] = x;
	this->_axis_cnt_channels[Axis::Y] = y;
	this->_axis_cnt_channels[Axis::Z] = z;
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
/// Method that makes an active wait until the counter value is reached
/// </summary>
/// <param name="step"></param>
/// <param name="channels"></param>
void AbstractCraneAxis::WaitUntilCounterReach(int step, short *channels) {
	this->_current_value = 0;
	this->_initial_value = 0;

	CntStartCount(this->_cnt_id, channels, 1);
	CntReadCount(this->_cnt_id, channels, 1, &_initial_value);
	CntReadCount(this->_cnt_id, channels, 1, &_current_value);

	// Wait until the expected value is reached
	while (std::abs(static_cast<long>(_current_value - _initial_value)) < std::abs(step)) {
		CntReadCount(this->_cnt_id, channels, 1, &_current_value);
	}

	CntStopCount(this->_cnt_id, channels, 1);
}

/// <summary>
/// Method that makes an active wait until the counter value is reached
/// </summary>
/// <param name="step"></param>
/// <param name="channels"></param>
void AbstractCraneAxis::WaitUntilCounterReach(int step, short *channels, bool *stop_signal) {
	this->_current_value = 0;
	this->_initial_value = 0;

	*stop_signal = false;
	CntStartCount(this->_cnt_id, channels, 1);
	CntReadCount(this->_cnt_id, channels, 1, &_initial_value);
	CntReadCount(this->_cnt_id, channels, 1, &_current_value);

	// Wait until the expected value is reached
	while (std::abs(static_cast<long>(_current_value - _initial_value)) < std::abs(step)) {
		CntReadCount(this->_cnt_id, channels, 1, &_current_value);
		if (*stop_signal)
			break;
	}

	*stop_signal = false;
	CntStopCount(this->_cnt_id, channels, 1);
}

/// <summary>
/// 
/// </summary>
/// <param name="axis"></param>
/// <returns></returns>
int AbstractCraneAxis::GetAioChannelFromAxis(Axis axis) {
	return this->_axis_aio_channels[axis];
}

/// <summary>
/// 
/// </summary>
/// <param name="axis"></param>
/// <returns></returns>
int AbstractCraneAxis::GetCntChannelFromAxis(Axis axis) {
	return this->_axis_cnt_channels[axis];
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <returns></returns>
int AbstractCraneAxis::GetCntValue(Axis a) {
	return std::abs(static_cast<int>(this->_current_value - this->_initial_value));
}