#include "stdafx.h"
#include "CraneMovementSystem.h"

/// <summary>
/// 
/// </summary>
/// <param name="aio_id"></param>
/// <param name="cnt_id"></param>
CraneMovementSystem::CraneMovementSystem(short aio_id, short cnt_id) {
	_aio_id = aio_id;
	_cnt_id = cnt_id;
}

/// <summary>
/// 
/// </summary>
CraneMovementSystem::~CraneMovementSystem() {
	// No code
}

/// <summary>
/// Halt the whole movement system
/// </summary>
void CraneMovementSystem::Halt() {
	for (auto &a : _axis)
		a.second->Halt();
}

/// <summary>
/// 
/// </summary>
/// <param name="key"></param>
/// <param name="axis"></param>
void CraneMovementSystem::SetAxis(char *key, std::shared_ptr<CraneAxis> axis) {
	_axis.insert(std::make_pair(key, std::move(axis)));
}
