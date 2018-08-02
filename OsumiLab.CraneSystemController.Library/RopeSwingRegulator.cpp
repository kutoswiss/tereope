#include "stdafx.h"
#include "RopeSwingRegulator.h"

std::mutex RopeSwingRegulator::mtx_capture;

/// <summary>
/// 
/// </summary>
RopeSwingRegulator::RopeSwingRegulator() {
	_x_voltage = 0.0;
	_y_voltage = 0.0;
}

/// <summary>
/// 
/// </summary>
RopeSwingRegulator::~RopeSwingRegulator() {
}

/// <summary>
/// 
/// </summary>
/// <param name="xframe"></param>
/// <param name="yframe"></param>
void RopeSwingRegulator::SetFrames(cv::Mat &xframe, cv::Mat &yframe) {
	_x_vision.SetFrame(xframe);
	_y_vision.SetFrame(yframe);
}

/// <summary>
/// 
/// </summary>
/// <param name="v"></param>
void RopeSwingRegulator::SetXVoltage(double v) {
	_x_voltage = this->TrimVoltage(v, this->kXMaxPeakVoltage, this->kXMaxPeakVoltage * -1);

	/*if ((_x_voltage > 0.2) && (_x_voltage < this->kXMinVoltage))
		_x_voltage = this->kXMinVoltage;
	else if ((_x_voltage < -0.2) && (_x_voltage >(this->kXMinVoltage * -1)))
		_x_voltage = (this->kXMinVoltage * -1);
	else if ((_x_voltage <= 0.2) && (_x_voltage >= -0.2))
		_x_voltage = 0.0;*/
}

/// <summary>
/// 
/// </summary>
/// <param name="v"></param>
void RopeSwingRegulator::SetYVoltage(double v) {
	_y_voltage = this->TrimVoltage(v, this->kYMaxPeakVoltage, this->kYMaxPeakVoltage * -1);

	/*if ((_y_voltage > 0.2) && (_y_voltage < this->kYMinVoltage))
		_y_voltage = this->kYMinVoltage;
	else if ((_y_voltage < -0.2) && (_y_voltage > (this->kYMinVoltage * -1)))
		_y_voltage = (this->kYMinVoltage * -1);
	else if((_y_voltage <= 0.2) && (_y_voltage >= -0.2))
		_y_voltage = 0.0;*/
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
double RopeSwingRegulator::GetXVoltage() const {
	return std::round(_x_voltage * 10) / 10;
}

/// <summary>
/// 
/// </summary>
/// <returns></returns>
double RopeSwingRegulator::GetYVoltage() const {
	return std::round(_y_voltage * 10) / 10;
}

/// <summary>
/// 
/// </summary>
/// <param name="crane"></param>
void RopeSwingRegulator::Regulate(Crane &crane, bool *stop) {
	std::ofstream f;
	f.open("rope-with-regulation.csv");

	while (true) {
		_x_vision.SetFrame(crane.XRopeCamera().GetMat(CV_8UC1));
		cv::imshow("X", _x_vision.GetFrame());

		_y_vision.SetFrame(crane.YRopeCamera().GetMat(CV_8UC1));
		cv::imshow("Y", _y_vision.GetFrame());

		/*_x_vision.Compute();
		_y_vision.Compute();

		this->SetXVoltage(this->AngleToXVoltage(_x_vision.GetAngle()));
		this->SetYVoltage(this->AngleToYVoltage(_y_vision.GetAngle()));

		cv::imshow("X", _x_vision.GetDecoratedFrame());
		cv::imshow("Y", _y_vision.GetDecoratedFrame());*/

		//crane.Fine()->X(this->GetXVoltage());
		//crane.Fine()->Y(this->GetYVoltage());

		if ((cv::waitKey(15) >= 0) || (*stop)) 
			break;
	}
	f.close();
	crane.Fine()->Halt();
}

/// <summary>
/// 
/// </summary>
/// <param name="crane"></param>
/// <param name="stop"></param>
void RopeSwingRegulator::RegulateX(Crane &crane, bool *stop) {
	double angle = 0.0;

	while (true) {
		_x_vision.SetFrame(crane.XRopeCamera().GetMat(CV_8UC1));
		_x_vision.Compute();
		angle = _x_vision.GetAngle();
		this->SetXVoltage(this->AngleToXVoltage(angle));
		cv::imshow("X", _x_vision.GetDecoratedFrame());
		crane.Fine()->X(this->GetXVoltage());

		if ((cv::waitKey(15) >= 0) || (*stop))
			break;
	}

	crane.Fine()->HaltX();
}

/// <summary>
/// 
/// </summary>
/// <param name="crane"></param>
/// <param name="stop"></param>
void RopeSwingRegulator::RegulateY(Crane &crane, bool *stop) {
	double angle = 0.0;

	while (true) {
		_y_vision.SetFrame(crane.YRopeCamera().GetMat(CV_8UC1));
		_y_vision.Compute();
		angle = _y_vision.GetAngle();
		this->SetYVoltage(this->AngleToYVoltage(angle));
		cv::imshow("Y", _y_vision.GetDecoratedFrame());
		crane.Fine()->Y(this->GetYVoltage());

		if ((cv::waitKey(15) >= 0) || (*stop))
			break;
	}

	crane.Fine()->HaltY();
}

/// <summary>
/// 
/// </summary>
void RopeSwingRegulator::Compute() {
	_x_vision.Compute();
	_y_vision.Compute();

	this->SetXVoltage(this->AngleToXVoltage(_x_vision.GetAngle()));
	this->SetYVoltage(this->AngleToYVoltage(_y_vision.GetAngle()));

	cv::imshow("X", _x_vision.GetDecoratedFrame());
	cv::imshow("Y", _y_vision.GetDecoratedFrame());
}

/// <summary>
/// 
/// </summary>
/// <param name="angle"></param>
/// <returns></returns>
double RopeSwingRegulator::AngleToXVoltage(double angle) {
	double v = angle * this->kXMaxPeakVoltage / this->kMaxAngle;
	if (v > 0)
		v += 0.2;
	return v;
}

/// <summary>
/// 
/// </summary>
/// <param name="angle"></param>
/// <returns></returns>
double RopeSwingRegulator::AngleToYVoltage(double angle) {
	double v = (angle * this->kYMaxPeakVoltage / this->kMaxAngle) * -1.0;
	return v;
}

/// <summary>
/// 
/// </summary>
/// <param name="v"></param>
/// <param name="max"></param>
/// <param name="min"></param>
/// <returns></returns>
double RopeSwingRegulator::TrimVoltage(double v, double max, double min) {
	double voltage = 0.0;

	if (v > max)
		voltage = max;
	else if (v < min)
		voltage = min;
	else
		voltage = v;

	return voltage;
}