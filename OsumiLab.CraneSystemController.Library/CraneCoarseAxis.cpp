#include "stdafx.h"
#include "CraneCoarseAxis.h"

/// <summary>
/// 
/// </summary>
CraneCoarseAxis::CraneCoarseAxis() : CraneCoarseAxis(-1, -1, -1, -1) {
}

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneCoarseAxis::CraneCoarseAxis(short aio_id, short cnt_id, int x_axis_channel, int y_axis_channel) {
	this->SetAioCntIDs(aio_id, cnt_id);
	this->SetAioChannels(x_axis_channel, y_axis_channel, -1);
}

/// <summary>
/// 
/// </summary>
CraneCoarseAxis::~CraneCoarseAxis() {
	this->Stop(Axis::X);

	this->_x_axis_thread.reset();
	this->_y_axis_thread.reset();
}

/// <summary>
/// Method to move the Axis 
/// </summary>
/// <param name="a">Axis to be moved</param>
/// <param name="step">Number of steps to move</param>
void CraneCoarseAxis::Move(Axis a, int step, double voltage) {
	switch (a) {
	case X:
		this->Stop(a);
		this->_x_axis_thread = std::make_unique<std::thread>(
			&CraneCoarseAxis::MoveXThread, this, std::ref(step), std::ref(voltage));
		break;
	case Y:
		this->Stop(a);
		this->_y_axis_thread = std::make_unique<std::thread>(
			&CraneCoarseAxis::MoveYThread, this, std::ref(step), std::ref(voltage));
		break;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
void CraneCoarseAxis::Stop(Axis a) {
	short aio = this->GetAioChannelFromAxis(a);
	AioSingleAoEx(this->_aio_id, aio, 0);

	switch (a) {
	case X:
		this->_x_stop_signal = true;
		if ((this->_x_axis_thread != nullptr) && this->_x_axis_thread->joinable())
			this->_x_axis_thread->join();
		break;
	case Y:
		this->_y_stop_signal = true;
		if ((this->_y_axis_thread != nullptr) && this->_y_axis_thread->joinable())
			this->_y_axis_thread->join();
		break;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::MoveXThread(int& step, double& voltage) {
	short aio = this->GetAioChannelFromAxis(Axis::X);
	short cnt = this->GetCntChannelFromAxis(Axis::X);

	this->SetVoltage(voltage);
	AioSingleAoEx(this->_aio_id, aio, this->_voltage * ((step < 0) ? 1 : -1));
	this->WaitUntilCounterReach(step, &cnt, &(this->_x_stop_signal));
	AioSingleAoEx(this->_aio_id, aio, 0);
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::MoveYThread(int& step, double& voltage) {
	short aio = this->GetAioChannelFromAxis(Axis::Y);
	short cnt = this->GetCntChannelFromAxis(Axis::Y);

	this->SetVoltage(voltage);
	AioSingleAoEx(this->_aio_id, aio, this->_voltage * ((step < 0) ? 1 : -1));
	this->WaitUntilCounterReach(step, &cnt, &(this->_y_stop_signal));
	AioSingleAoEx(this->_aio_id, aio, 0);
}
