#pragma once
#include "CraneAxis.h"
#include <map>
#include <memory>

class CraneMovementSystem
{
public:
	// Virtual methods
	virtual ~CraneMovementSystem() {};
	
	// Public methods
	void Halt();

protected:
	// Virtual methods
	virtual void Move(char *axis_key, int step, double voltage = 0) = 0;

	// Protected methods
	void SetAxis(char *key, std::shared_ptr<CraneAxis> axis);

	// Private members
	short _aio_id;
	short _cnt_id;
	std::map<char*, std::shared_ptr<CraneAxis>> _axis;
};

