#pragma once
#include "CraneAxis.h"
#include <map>
#include <memory>

class CraneMovementSystem
{
public:
	CraneMovementSystem(short aio_id, short cnt_id);
	~CraneMovementSystem();
	
	// Public methods
	void Halt();

protected:
	// Protected methods
	void SetAxis(char *key, std::shared_ptr<CraneAxis> axis);

	// Private members
	short _aio_id;
	short _cnt_id;
	std::map<char*, std::shared_ptr<CraneAxis>> _axis;
};

