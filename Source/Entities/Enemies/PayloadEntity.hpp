#pragma once
#ifndef __PAYLOADENTITY_H
#define __PAYLOADENTITY_H
#include "BossEntity.hpp"
#include "../../Utils/Constant.hpp"

class PayloadEntity : public BossEntity {
public:
	enum struct INNERFSM {
		JUMP,LAND,ATTACK, TOTAL
	};
protected:
	INNERFSM inner_state;
	GameObjectEntity* ground;
	float land_cooldown{ BOSS2SHOOTCOOLDOWN };
	float land_timer;
	float attack_range;
	int base_projectiles, extra_projectiles;
public:
	PayloadEntity(AEVec2 pos);
	~PayloadEntity() override;
	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void Render() override;
	void OnCollide(GameObjectEntity* go) override;
	// Virtual methods to Enemy Base class
	void OnIdle(const f32& dt) override;
	void OnPatrol(const f32& dt) override;
	void OnChase(const f32& dt) override;
	void OnStun(const f32& dt) override;
	void OnDead(const f32& dt) override;
};



#endif