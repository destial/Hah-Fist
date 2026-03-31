#pragma once
#ifndef __SPIDERENTITY_H
#define __SPIDERENTITY_H
#include "EnemyEntity.hpp"
class SpiderEntity : public EnemyEntity {
protected:
	bool bSpawnHatchlings;
public:
	SpiderEntity(AEVec2 pos, f32 speed = 10.f, bool spawnHatchlings = true);
	~SpiderEntity() override;
	void PostUpdate(const f32& dt) override;
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class
	void OnHit() override;
	void OnIdle(const f32& dt) override;
	void OnDead() override;
};



#endif
