#pragma once
#ifndef __MISSILEPROJECTILE_HPP
#define __MISSILEPROJECTILE_HPP
#include "BaseProjectile.hpp"

class MissileProjectile : public BaseProjectile {
private:
	f32 slowStartTime{ 1.0f };

public:
	MissileProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own);
	virtual ~MissileProjectile();
};

#endif