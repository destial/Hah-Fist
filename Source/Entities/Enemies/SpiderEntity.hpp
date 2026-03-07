#pragma once
#ifndef __SPIDERENTITY_H
#define __SPIDERENTITY_H
#include "EnemyEntity.hpp"
class SpiderEntity : public EnemyEntity {
protected:
	GameObjectEntity* ground;
	bool bSpawnHatchlings;
public:
	SpiderEntity(AEVec2 pos, f32 speed = 10.f, bool spawnHatchlings = true);
	~SpiderEntity() override;
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
