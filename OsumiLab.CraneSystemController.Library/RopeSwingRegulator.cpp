#include "stdafx.h"
#include "RopeSwingRegulator.h"

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
		_y_vision.SetFrame(crane.YRopeCamera().GetMat(CV_8UC1));

		_x_vision.Compute();
		_y_vision.Compute();

		this->SetXVoltage(this->AngleToXVoltage(_x_vision.GetAngle()));
		this->SetYVoltage(this->AngleToYVoltage(_y_vision.GetAngle()));

		cv::imshow("X", _x_vision.GetDecoratedFrame());
		cv::imshow("Y", _y_vision.GetDecoratedFrame());

		crane.FineAxis().SetAxisVoltage(Axis::X, this->GetXVoltage());
		crane.FineAxis().SetAxisVoltage(Axis::Y, this->GetYVoltage());

		if ((cv::waitKey(15) >= 0) || (*stop)) break;
	}
	f.close();

	crane.FineAxis().Stop(Axis::X);
	crane.FineAxis().Stop(Axis::Y);
}

/// <summary>
/// 
/// </summary>
/// <param name="angle"></param>
/// <returns></returns>
double RopeSwingRegulator::AngleToXVoltage(double angle) {
	return angle * this->kXMaxPeakVoltage / this->kMaxAngle;
}

/// <summary>
/// 
/// </summary>
/// <param name="angle"></param>
/// <returns></returns>
double RopeSwingRegulator::AngleToYVoltage(double angle) {
	return angle * this->kYMaxPeakVoltage / this->kMaxAngle;
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