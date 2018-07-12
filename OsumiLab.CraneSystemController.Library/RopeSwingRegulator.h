#pragma once
#include "RopeSideVision.h"
#include "Crane.h"

class RopeSwingRegulator
{
public:
	static std::mutex mtx_capture;

	// Constants
	const double kXMaxPeakVoltage = 2.0;
	const double kYMaxPeakVoltage = 2.5;
	const double kXMinTriggerVoltage = 0.5;
	const double kYMinTriggerVoltage = 0.5;
	const double kMaxAngle = 4.5;

	// Ctor/Dtor
	RopeSwingRegulator();
	~RopeSwingRegulator();

	// Getters/Setters
	void SetFrames(cv::Mat &xframe, cv::Mat &yframe);
	void SetXVoltage(double v);
	void SetYVoltage(double v);
	double GetXVoltage() const;
	double GetYVoltage() const;

	// Public method
	void Regulate(Crane &crane, bool *stop);
	void RegulateX(Crane &crane, bool *stop);
	void RegulateY(Crane &crane, bool *stop);
	void Compute();

private:
	// Private methods
	double AngleToXVoltage(double angle);
	double AngleToYVoltage(double angle);
	double TrimVoltage(double v, double max, double min);

	// Private members
	RopeSideVision _x_vision;
	RopeSideVision _y_vision;
	double _x_voltage;
	double _y_voltage;
};

