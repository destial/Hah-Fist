#pragma once
#ifndef __MISSILEPROJECTILE_HPP
#define __MISSILEPROJECTILE_HPP
#include "BaseProjectile.hpp"
class MissileProjectile : public BaseProjectile
{
public:
	MissileProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own);
	void PreUpdate(const f32& dt);
private:
	f32 slowStartTime{ 1.0f };
	
};


#endif