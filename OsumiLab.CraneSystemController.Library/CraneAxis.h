#pragma once
#include <windows.h>
#include <cstdlib>
#include <memory>
#include <thread>
#include <mutex>
#include "Caio.h"
#include "CCnt.h"
#include "CraneSettings.h"

class CraneAxis
{
public:
	// Public static members
	static std::mutex mtx_move;

	// Ctor/Dtor
	CraneAxis(short aio_id, short cnt_id, short aio_ch, short cnt_ch, short enable_ch);
	~CraneAxis();

	// Public methods
	void Enable();
	void Disable();
	void SetVoltage(double voltage);
	void SetMaxVoltage(double voltage);
	void Halt();
	void Move(int step, double voltage);
	void MoveThread(int step, double voltage);
	void MoveJoinThread();
	void WaitUntilCounterReach(int step);
	int GetCntValue();

private:
	// Private constants
	const double kEnableVoltage = 5.0;
	const double kDisableVoltage = 0.0;
	const double kDefaultMaxVoltage = 2.5;

	// Private methods
	double TrimVoltage(double voltage);
	void MoveThreadImpl(int &step, double &voltage);

	// Private members
	short _aio_id;
	short _cnt_id;
	short _aio_channel;
	short _cnt_channel;
	short _enable_channel;

	DWORD _current_cnt_value;
	DWORD _initial_cnt_value;

	double _max_voltage;
	double _previous_voltage = 9999;
	bool _cnt_halt_signal = false;
	bool _enable = false;

	std::unique_ptr<std::thread> _task;
};

