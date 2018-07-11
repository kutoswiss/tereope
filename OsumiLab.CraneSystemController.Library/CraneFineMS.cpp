#include "stdafx.h"
#include "CraneFineMS.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneFineMS::CraneFineMS(short aio_id, short cnt_id) : CraneMovementSystem(aio_id, cnt_id) {
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
CraneFineMS::~CraneFineMS() {
	this->HaltX();
	this->HaltY();
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneFineMS::X(double voltage) {
	_axis[this->kXAxisKey]->Enable();
	_axis[this->kXAxisKey]->SetVoltage(voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="voltage"></param>
void CraneFineMS::Y(double voltage) {
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
void CraneFineMS::X(int step, double voltage) {
	// Positive voltage -> Move on right
	// Negative voltage -> Move on left
	_axis[this->kXAxisKey]->Move(step, voltage);
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneFineMS::Y(int step, double voltage) {
	// Positive voltage -> Move on top
	// Negative voltage -> Move on bottom
	voltage *= -1;
	_axis[this->kYAxisKey]->Move(step, voltage);
}

/// <summary>
/// 
/// </summary>
void CraneFineMS::HaltX() {
	_axis[this->kXAxisKey]->Halt();
}

/// <summary>
/// 
/// </summary>
void CraneFineMS::HaltY() {
	_axis[this->kYAxisKey]->Halt();
}
