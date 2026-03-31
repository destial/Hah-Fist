#pragma once
#ifndef __SPIKEPROJECTILE_HPP
#define __SPIKEPROJECTILE_HPP
#include "BaseProjectile.hpp"

class SpikeProjectile : public BaseProjectile {
private:
	f32 slowStartTime{ 1.0f };

public:
	SpikeProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own);
	virtual ~SpikeProjectile();
};

#endif