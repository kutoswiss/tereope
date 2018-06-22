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
	this->SetAioChannels(x_axis_channel, y_axis_channel, -1);
}

/// <summary>
/// 
/// </summary>
CraneFineAxis::~CraneFineAxis() {
}

/// <summary>
/// Enable coarse axis
/// </summary>
/// <param name="a"></param>
void CraneFineAxis::Enable(Axis a) {
	// Enable the axis
	this->SetEnableVoltage(a, 5.0);
}

/// <summary>
/// Disable coarse axis
/// </summary>
/// <param name="a"></param>
void CraneFineAxis::Disable(Axis a) {
	this->SetEnableVoltage(a, 0.0);
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="voltage"></param>
void CraneFineAxis::SetEnableVoltage(Axis a, const double voltage) {
	switch (a) {
	case X: AioSingleAoEx(this->_aio_id, kXEnableChannel, voltage); break;
	case Y: AioSingleAoEx(this->_aio_id, kYEnableChannel, voltage); break;
	}
}


/// <summary>
/// Method to move the Axis 
/// </summary>
/// <param name="a">Axis to be moved</param>
/// <param name="step">Number of steps to move</param>
void CraneFineAxis::Move(Axis a, int step, double voltage) {
	short aio = this->GetAioChannelFromAxis(a);
	short cnt = this->GetCntChannelFromAxis(a);

	if (a == Axis::X)
		step *= -1;

	this->SetVoltage(voltage);
	this->Enable(a);
	AioSingleAoEx(this->_aio_id, aio, this->_voltage * ((step < 0) ? 1 : -1));
	this->WaitUntilCounterReach(step, &cnt);
	AioSingleAoEx(this->_aio_id, aio, 0);
	this->Disable(a);
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
void CraneFineAxis::Stop(Axis a) {
	short aio = this->GetAioChannelFromAxis(a);
	AioSingleAoEx(this->_aio_id, aio, 0);
	this->Disable(a);
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="voltage"></param>
void CraneFineAxis::SetAxisVoltage(Axis a, double voltage) {
	short aio = this->GetAioChannelFromAxis(a);
	this->Enable(a);
	AioSingleAoEx(this->_aio_id, aio, voltage);
}
