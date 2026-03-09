#pragma once
#ifndef __GRAPPLEFISTENTITY_H
#define __GRAPPLEFISTENTITY_H
#include "../WeaponEntity.hpp"

class GrappleFistWeapon : public Weapon {
protected:
	f32 grappleSpeedMult{ 50.0f };
	enum GRAPPLE_STATE {
		INACTIVE,
		SHOOTING,
		RETURNING,
		HOOKING_OBJECT_TO_PLAYER,
		HOOKING_PLAYER_TO_STATIC_OBJECT,
		HOOKING_PLAYER_TO_DYNAMIC_OBJECT,
	}grappleState{ INACTIVE };
	f32 travelDuration{ 0.0f };
	GameObjectEntity* grappledObject{ nullptr };
	AEVec2 snapshottedAttackDirection;
public:
	GrappleFistWeapon(AEVec2 pos, GameObjectEntity* player);
	~GrappleFistWeapon();

	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void Render() override;
	void OnCollide(GameObjectEntity* go) override;
	void Attack() override;
	void ResetWeapon();
};
#endif