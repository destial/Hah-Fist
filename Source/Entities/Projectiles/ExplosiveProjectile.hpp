#pragma once
#ifndef __EXPLOSIVEPROJECTILE_HPP
#define __EXPLOSIVEPROJECTILE_HPP
#include "BaseProjectile.hpp"
class ExplosiveProjectile : public BaseProjectile
{
public:
	ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own);
	void PreUpdate(const f32& dt);
private:
	f32 slowStartTime{ 1.0f };
	
};


#endif