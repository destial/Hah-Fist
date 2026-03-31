#pragma once
#ifndef __TROOPERENTITY_H
#define __TROOPERENTITY_H
#include "EnemyEntity.hpp"
class TrooperEntity : public EnemyEntity {
public:
	TrooperEntity(AEVec2 pos, f32 speed = 10.f);
	~TrooperEntity() override;
	void PostUpdate(const f32& dt) override;
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class
	void OnIdle(const f32& dt) override;
	void OnDead() override;
};
#endif