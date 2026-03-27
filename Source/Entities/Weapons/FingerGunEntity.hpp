#pragma once
#ifndef __FINGERGUNENTITY_H
#define __FINGERGUNENTITY_H
#include "../WeaponEntity.hpp"

class FingerGunWeapon : public Weapon {
protected:

public:
	FingerGunWeapon(AEVec2 pos);
	~FingerGunWeapon();

	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void OnCollide(GameObjectEntity* go) override;
	void Render() override;
	void Attack() override;
	void ResetWeapon();
};
#endif
