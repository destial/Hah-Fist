#pragma once
#ifndef __EXPLOSIVEPROJECTILE_HPP
#define __EXPLOSIVEPROJECTILE_HPP
#include "BaseProjectile.hpp"
class ExplosiveProjectile : public BaseProjectile
{
public:
	ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own, bool lerp = false);

	void Update(const f32& dt) override;
	void OnHit(GameObjectEntity* other) override;
	void OnExpire() override;
	void Explode();
private:
	f32 slowStartTime{ 0.5f };
	f32 explosionRadius{ 5.f };   
	f32 explosionDamage;          
	bool exploded{ false };       
	
};


#endif