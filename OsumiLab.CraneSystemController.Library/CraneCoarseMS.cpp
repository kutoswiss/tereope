#include "stdafx.h"
#include "CraneCoarseMS.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneCoarseMS::CraneCoarseMS(short aio_id, short cnt_id) : CraneMovementSystem(aio_id, cnt_id) {
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
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneCoarseMS::X(double voltage) {
	// Positive voltage -> Move on right
	// Negative voltage -> Move on left
	voltage *= -1;
	_axis[this->kXAxisKey]->Enable();
	_axis[this->kXAxisKey]->SetVoltage(voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneCoarseMS::Y(double voltage) {
	// Positive voltage -> Move on top
	// Negative voltage -> Move on bottom
	voltage *= -1;
	_axis[this->kYAxisKey]->Enable();
	_axis[this->kYAxisKey]->SetVoltage(voltage);
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
	_axis[this->kXAxisKey]->Move(step, voltage);
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
	_axis[this->kYAxisKey]->Move(step, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::XThread(int step, double voltage) {
	voltage = voltage * ((step < 0) ? 1 : -1);
	_axis[this->kXAxisKey]->Halt();
	_axis[this->kXAxisKey]->MoveThread(step, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseMS::YThread(int step, double voltage) {
	voltage = voltage * ((step < 0) ? 1 : -1);
	_axis[this->kYAxisKey]->Halt();
	_axis[this->kYAxisKey]->MoveThread(step, voltage);
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::XJoinThread() {
	_axis[this->kXAxisKey]->MoveJoinThread();
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::YJoinThread() {
	_axis[this->kYAxisKey]->MoveJoinThread();
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::HaltX() {
	_axis[this->kXAxisKey]->Halt();
}

/// <summary>
/// 
/// </summary>
void CraneCoarseMS::HaltY() {
	_axis[this->kYAxisKey]->Halt();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
int CraneCoarseMS::GetXCntValue() {
	return _axis[this->kXAxisKey]->GetCntValue();
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
int CraneCoarseMS::GetYCntValue() {
	return _axis[this->kYAxisKey]->GetCntValue();
}
