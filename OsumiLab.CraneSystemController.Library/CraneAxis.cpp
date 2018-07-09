#include "stdafx.h"
#include "CraneAxis.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
/// <param name="enable_id"></param>
CraneAxis::CraneAxis(short aio_id, short cnt_id, short aio_ch, short cnt_ch, short enable_ch) {
	_aio_id = aio_id;
	_cnt_id = cnt_id;
	_aio_channel = aio_ch;
	_cnt_channel = cnt_ch;
	_enable_channel = enable_ch;
	this->SetMaxVoltage(kDefaultMaxVoltage);
}

/// <summary>
/// 
/// </summary>
CraneAxis::~CraneAxis() {
	// No code
}

/// <summary>
/// 
/// </summary>
void CraneAxis::Enable() {
	AioSingleAoEx(_aio_id, _enable_channel, -10.0);
	AioSingleAoEx(_aio_id, _enable_channel, this->kEnableVoltage);
}

/// <summary>
/// 
/// </summary>
void CraneAxis::Disable() {
	AioSingleAoEx(_aio_id, _enable_channel, this->kDisableVoltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneAxis::SetVoltage(double voltage) {
	voltage = this->TrimVoltage(voltage);
	AioSingleAoEx(_aio_id, _aio_channel, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneAxis::SetMaxVoltage(double voltage) {
	_max_voltage = voltage;
}

/// <summary>
/// 
/// </summary>
void CraneAxis::Halt() {
	_cnt_halt_signal = true;
	this->SetVoltage(0.0);
	this->Disable();
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="channels"></param>
/// <param name="stop_signal"></param>
void CraneAxis::WaitUntilCounterReach(int step) {
	_current_cnt_value = 0;
	_initial_cnt_value = 0;

	_cnt_halt_signal = false;
	CntStartCount(this->_cnt_id, &_cnt_channel, 1);
	CntReadCount(this->_cnt_id, &_cnt_channel, 1, &_initial_cnt_value);
	CntReadCount(this->_cnt_id, &_cnt_channel, 1, &_current_cnt_value);

	// Wait until the expected value is reached
	while (std::abs(static_cast<long>(_current_cnt_value - _initial_cnt_value)) < std::abs(step)) {
		CntReadCount(this->_cnt_id, &_cnt_channel, 1, &_current_cnt_value);
		if (_cnt_halt_signal)
			break;
	}

	_cnt_halt_signal = false;
	CntStopCount(this->_cnt_id, &_cnt_channel, 1);
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
/// <returns></returns>
double CraneAxis::TrimVoltage(double voltage) {
	double v = voltage;

	if (v > _max_voltage)
		v = _max_voltage;

	if (v < (_max_voltage * -1.0))
		v = _max_voltage * -1.0;

	return v;
}
