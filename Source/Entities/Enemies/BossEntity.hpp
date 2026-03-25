#pragma once
#ifndef __BOSSENTITY_H
#define __BOSSENTITY_H
#include "EnemyEntity.hpp"
#include "../../Utils/Constant.hpp"
class BossEntity : public EnemyEntity {
protected:
	GameObjectEntity* ground;
	bool bossActivated;
	AEVec2 bossRoomCenter;
public:

	BossEntity(AEVec2 pos);
	virtual ~BossEntity();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render() override;
	virtual void OnCollide(GameObjectEntity* go);
	//Default Boss functions
	void ShootProjectile(float healthRatio, AEVec2 Pos, AEVec2 shootDir);
	float GetLowHealthFactor();
	bool GetBossActivated();
	void SetBossActivation(bool activated);
	AEVec2 GetBossRoomCenter();
	// Virtual methods to Enemy Base class
	virtual void OnHit() override;
	virtual void OnIdle(const f32& dt);
	virtual void OnPatrol(const f32& dt);
	virtual void OnAttack(const f32& dt);
	virtual void OnChase(const f32& dt);
	virtual void OnStun(const f32& dt);
	virtual void OnDead(const f32& dt);
};



#endif