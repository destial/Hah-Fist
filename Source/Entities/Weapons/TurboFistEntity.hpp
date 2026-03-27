#pragma once
#ifndef __TURBOFISTENTITY_H
#define __TURBOFISTENTITY_H
#include "../WeaponEntity.hpp"

class TurboFistWeapon : public Weapon {
protected:
	f32 max_dash_time = 0.25f;
	f32 dash_timer = 0.0f;

	bool dashing = false;
	f32 player_original_mass = 0.0f;
public:

	TurboFistWeapon(AEVec2 pos, f32 og_mass);
	~TurboFistWeapon();

	void Update(const f32& dt) override;
	void Render() override;
	void OnCollide(GameObjectEntity* go) override;
	void Attack() override;
	float GetCurrentAttackStrength();
	void ResetWeapon();
};

#endif