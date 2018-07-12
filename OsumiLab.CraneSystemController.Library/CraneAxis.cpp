#include "stdafx.h"
#include "CraneAxis.h"

std::mutex CraneAxis::mtx_move;

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
	this->Halt();
	_task.reset();
}

/// <summary>
/// 
/// </summary>
void CraneAxis::Enable() {
	if(_enable == false) {
		AioSingleAoEx(_aio_id, _enable_channel, -10.0);
		AioSingleAoEx(_aio_id, _enable_channel, this->kEnableVoltage);
		_enable = true;
	}
}

/// <summary>
/// 
/// </summary>
void CraneAxis::Disable() {
	AioSingleAoEx(_aio_id, _enable_channel, this->kDisableVoltage);
	_enable = false;
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneAxis::SetVoltage(double voltage) {
	voltage = this->TrimVoltage(voltage);
	if(voltage != _previous_voltage){
		CraneAxis::mtx_move.lock();
		AioSingleAoEx(_aio_id, _aio_channel, voltage);
		_previous_voltage = voltage;
		CraneAxis::mtx_move.unlock();
	}
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
	this->MoveJoinThread();
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneAxis::Move(int step, double voltage) {
	this->Enable();
	this->SetVoltage(voltage);
	this->WaitUntilCounterReach(step);
	this->SetVoltage(0.0);
	this->Disable();
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneAxis::MoveThread(int step, double voltage) {
	this->Halt();
	_task = std::make_unique<std::thread>(
		&CraneAxis::MoveThreadImpl,
		this,
		std::ref(step),
		std::ref(voltage));
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneAxis::MoveThreadImpl(int &step, double &voltage) {
	this->Move(step, voltage);
}

/// <summary>
/// 
/// </summary>
void CraneAxis::MoveJoinThread() {
	if ((_task != nullptr) && _task->joinable())
		_task->join();
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
/// <returns></returns>
int CraneAxis::GetCntValue() {
	return std::abs(static_cast<int>(_current_cnt_value - _initial_cnt_value));
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
