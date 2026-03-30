#pragma once
#ifndef __TITANENTITY_H
#define __TITANENTITY_H
#include "BossEntity.hpp"
#include "../../Utils/Constant.hpp"
class TitanEntity : public BossEntity {
protected:
	GameObjectEntity* ground;
	float shoot_cooldown{ BOSS1SHOOTCOOLDOWN };
	float shoot_timer;
	float attack_range,jump_x,jump_y;
	int base_projectiles, extra_projectiles;
public:
	
	TitanEntity(AEVec2 pos);
	~TitanEntity() override;
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