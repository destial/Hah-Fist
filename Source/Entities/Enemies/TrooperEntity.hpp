#pragma once
#ifndef __TROOPERENTITY_H
#define __TROOPERENTITY_H
#include "EnemyEntity.hpp"
class TrooperEntity : public EnemyEntity {
public:
	TrooperEntity(AEVec2 pos, f32 speed = 10.f);
	~TrooperEntity() override;
	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void Render() override;
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class
	void OnHit() override;
	void OnIdle(const f32& dt) override;
	void OnPatrol(const f32& dt) override;
	void OnChase(const f32& dt) override;
	void OnStun(const f32& dt) override;
	void OnDead(const f32& dt) override;
};
#endif