#include "stdafx.h"
#include "CraneFineMS.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneFineMS::CraneFineMS(short aio_id, short cnt_id) {
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
void CraneFineMS::HaltX() {
	_axis[this->kXAxisKey]->Halt();
}

/// <summary>
/// 
/// </summary>
void CraneFineMS::HaltY() {
	_axis[this->kYAxisKey]->Halt();
}

/// <summary>
/// 
/// </summary>
/// <param name="axis_key"></param>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneFineMS::Move(char *axis_key, int step, double voltage) {

}
