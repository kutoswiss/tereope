#include "stdafx.h"
#include "CraneMovementSystem.h"

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
