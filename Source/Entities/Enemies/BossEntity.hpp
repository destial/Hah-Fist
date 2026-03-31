#pragma once
#ifndef __BOSSENTITY_H
#define __BOSSENTITY_H
#include "EnemyEntity.hpp"

class BossEntity : public EnemyEntity {
protected:
	GameObjectEntity* ground;
	bool boss_activated;
	AEVec2 boss_room_center;
public:
	BossEntity(AEVec2 pos);
	virtual ~BossEntity();

	//Default Boss functions
	void ShootProjectile(float healthRatio, AEVec2 Pos, AEVec2 shootDir);
	float GetLowHealthFactor();
	bool GetBossActivated() const;
	void SetBossActivation(bool activated);
	AEVec2 GetBossRoomCenter() const;
	// Virtual methods to Enemy Base class

	virtual void OnIdle(const f32& dt);
	virtual void OnPatrol(const f32& dt);
	virtual void OnChase(const f32& dt);
	virtual void OnAttack(const f32& dt);
	virtual void OnStun(const f32& dt);
	virtual void OnDead();
};

#endif