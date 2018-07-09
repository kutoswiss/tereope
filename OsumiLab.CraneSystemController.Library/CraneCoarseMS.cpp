#include "stdafx.h"
#include "CraneCoarseMS.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneCoarseMS::CraneCoarseMS(short aio_id, short cnt_id) {
	_aio_id = aio_id;
	_cnt_id = cnt_id;

	// Setup the X axis
	this->SetAxis(this->kXAxisKey, 
		std::make_shared<CraneAxis>(_aio_id, _cnt_id, 
			this->kXAxisAioChannel, 
			this->kXAxisCntChannel, 
			this->kXAxisEnableChannel));

	// Setup the Y axis
	this->SetAxis(this->kYAxisKey,
		std::make_shared<CraneAxis>(_aio_id, _cnt_id,
			this->kYAxisAioChannel,
			this->kYAxisCntChannel,
			this->kYAxisEnableChannel));
}

/// <summary>
/// 
/// </summary>
CraneCoarseMS::~CraneCoarseMS() {
	this->HaltX();
	this->HaltY();

	_x_thread.reset();
	_y_thread.reset();
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::X(int step, double voltage) {
	// Positive voltage -> Move on right
	// Negative voltage -> Move on left
	voltage = voltage * ((step < 0) ? 1 : -1);

	_axis[this->kXAxisKey]->Enable();
	_axis[this->kXAxisKey]->SetVoltage(voltage);
	_axis[this->kXAxisKey]->WaitUntilCounterReach(step);
	_axis[this->kXAxisKey]->SetVoltage(0.0);
	_axis[this->kXAxisKey]->Disable();
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::Y(int step, double voltage) {
	// Positive voltage -> Move on top
	// Negative voltage -> Move on bottom
	voltage = voltage * ((step < 0) ? 1 : -1);

	_axis[this->kYAxisKey]->Enable();
	_axis[this->kYAxisKey]->SetVoltage(voltage);
	_axis[this->kYAxisKey]->WaitUntilCounterReach(step);
	_axis[this->kYAxisKey]->SetVoltage(0.0);
	_axis[this->kYAxisKey]->Disable();
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::XThread(int step, double voltage) {
	this->HaltX();
	_x_thread = std::make_unique<std::thread>(
		&CraneCoarseMS::XThreadImpl, 
		this, 
		std::ref(step), 
		std::ref(voltage));
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::YThread(int step, double voltage) {
	this->HaltY();
	_y_thread = std::make_unique<std::thread>(
		&CraneCoarseMS::YThreadImpl,
		this,
		std::ref(step),
		std::ref(voltage));
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::XThreadImpl(int &step, double &voltage) {
	this->X(step, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::YThreadImpl(int &step, double &voltage) {
	this->Y(step, voltage);
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::XJoinThread() {
	if ((_x_thread != nullptr) && _x_thread->joinable())
		_x_thread->join();
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::YJoinThread() {
	if ((_y_thread != nullptr) && _y_thread->joinable())
		_y_thread->join();
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::HaltX() {
	_axis[this->kXAxisKey]->Halt();
	this->XJoinThread();
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::HaltY() {
	_axis[this->kYAxisKey]->Halt();
	this->YJoinThread();
}

/// <summary>
/// 
/// </summary>
/// <param name="axis_key"></param>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::Move(char *axis_key, int step, double voltage) {

}
