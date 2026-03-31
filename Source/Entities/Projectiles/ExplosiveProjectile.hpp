#pragma once
#ifndef __EXPLOSIVEPROJECTILE_HPP
#define __EXPLOSIVEPROJECTILE_HPP
#include "BaseProjectile.hpp"

class ExplosiveProjectile : public BaseProjectile {
public:
	ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own, bool lerp = false);
	virtual ~ExplosiveProjectile();
	virtual void OnHit(GameObjectEntity* other);
	void OnExpire() override;
};

#endif