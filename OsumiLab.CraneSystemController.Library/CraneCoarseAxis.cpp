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
	this->Stop(Axis::Y);

	this->_x_axis_thread.reset();
	this->_y_axis_thread.reset();
}

/// <summary>
/// Enable coarse axis
/// </summary>
/// <param name="a"></param>
void CraneCoarseAxis::Enable(Axis a) {
	// Little hack against remaining voltage
	// It works for the moment.
	this->SetEnableVoltage(a, -10.0); 

	// Enable the axis
	this->SetEnableVoltage(a, 5.0);
}

/// <summary>
/// Disable coarse axis
/// </summary>
/// <param name="a"></param>
void CraneCoarseAxis::Disable(Axis a) {
	this->SetEnableVoltage(a, 0.0);
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::SetEnableVoltage(Axis a, const double voltage) {
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
void CraneCoarseAxis::Move(Axis a, int step, double voltage) {
	switch (a) {
	case X: this->MoveX(step, voltage); break;
	case Y: this->MoveY(step, voltage); break;
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::MoveThread(Axis a, int step, double voltage) {
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
/// <returns></returns>
void CraneCoarseAxis::WaitUntilFinish(Axis a) {
	switch (a) {
	case X:
		if ((this->_x_axis_thread != nullptr) && this->_x_axis_thread->joinable()) 
			this->_x_axis_thread->join();
		
		break;
	case Y: 
		if ((this->_y_axis_thread != nullptr) && this->_y_axis_thread->joinable())
			this->_y_axis_thread->join();
	};
}

/// <summary>
/// 
/// </summary>
/// <param name="a"></param>
void CraneCoarseAxis::Stop(Axis a) {
	short aio = this->GetAioChannelFromAxis(a);
	AioSingleAoEx(this->_aio_id, aio, 0);

	switch (a) {
	case X: this->_x_stop_signal = true; break;
	case Y: this->_y_stop_signal = true; break;
	}

	this->WaitUntilFinish(a);
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::MoveX(int step, double voltage) {
	double ramp_voltage = 0.0;
	int ramp_steps = 0;
	int stable_steps = 0;
	short aio = this->GetAioChannelFromAxis(Axis::X);
	short cnt = this->GetCntChannelFromAxis(Axis::X);

	this->SetVoltage(voltage);
	double v = this->_voltage * ((step < 0) ? 1 : -1);

	this->Enable(Axis::X); 
	ramp_steps = (step / 4) / 10; 
	stable_steps = step - (ramp_steps * 10 * 2);
	for (int i = 1; i <= 10; i++) {
		ramp_voltage = (v / 10) * i;
		AioSingleAoEx(this->_aio_id, aio, ramp_voltage);
		this->WaitUntilCounterReach(ramp_steps, &cnt, &(this->_x_stop_signal));
	}

	AioSingleAoEx(this->_aio_id, aio, v);
	this->WaitUntilCounterReach(stable_steps, &cnt, &(this->_x_stop_signal));

	for (int i = 10; i > 0; i--) {
		ramp_voltage = (v / 10) * i;
		AioSingleAoEx(this->_aio_id, aio, ramp_voltage);
		this->WaitUntilCounterReach(ramp_steps, &cnt, &(this->_x_stop_signal));
	}
	AioSingleAoEx(this->_aio_id, aio, 0);

	this->Disable(Axis::X);
}

/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::MoveY(int step, double voltage) {
	double ramp_voltage = 0.0;
	int ramp_steps = 0;
	int stable_steps = 0;
	short aio = this->GetAioChannelFromAxis(Axis::Y);
	short cnt = this->GetCntChannelFromAxis(Axis::Y);

	this->SetVoltage(voltage);
	double v = this->_voltage * ((step < 0) ? 1 : -1);

	//this->Enable(Axis::Y);
	//ramp_steps = (step / 4) / 10;
	//stable_steps = step - (ramp_steps * 10 * 2);
	//for (int i = 1; i <= 10; i++) {
	//	ramp_voltage = (v / 10) * i;
	//	AioSingleAoEx(this->_aio_id, aio, ramp_voltage);
	//	this->WaitUntilCounterReach(ramp_steps, &cnt, &(this->_y_stop_signal));
	//}

	//AioSingleAoEx(this->_aio_id, aio, v);
	//this->WaitUntilCounterReach(stable_steps, &cnt, &(this->_y_stop_signal));

	//for (int i = 10; i > 0; i--) {
	//	ramp_voltage = (v / 10) * i;
	//	AioSingleAoEx(this->_aio_id, aio, ramp_voltage);
	//	this->WaitUntilCounterReach(ramp_steps, &cnt, &(this->_y_stop_signal));
	//}
	//AioSingleAoEx(this->_aio_id, aio, 0);
	//this->Disable(Axis::Y);

	this->Enable(Axis::Y);
	AioSingleAoEx(this->_aio_id, aio, v);
	this->WaitUntilCounterReach(step, &cnt, &(this->_y_stop_signal));
	AioSingleAoEx(this->_aio_id, aio, 0);
	this->Disable(Axis::Y);
}


/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::MoveXThread(int& step, double& voltage) {
	this->MoveX(step, voltage);
}



/// <summary>
/// 
/// </summary>
/// <param name="step"></param>
/// <param name="voltage"></param>
void CraneCoarseAxis::MoveYThread(int& step, double& voltage) {
	this->MoveY(step, voltage);
}
