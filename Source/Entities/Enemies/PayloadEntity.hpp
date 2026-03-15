#pragma once
#ifndef __PAYLOADENTITY_H
#define __PAYLOADENTITY_H
#include "EnemyEntity.hpp"
#include "../../Utils/Constant.hpp"

class PayloadEntity : public EnemyEntity {
public:
	enum struct INNERFSM {
		JUMP,LAND,ATTACK, TOTAL
	};
protected:
	INNERFSM innerState;
	GameObjectEntity* ground;
	float landCooldown{ BOSS2SHOOTCOOLDOWN };
	float landTimer;
	bool bossActivated;
	float attackRange, jumpX, jumpY;
	int baseProjectiles, extraProjectiles;
	AEVec2 bossRoomCenter;
public:
	PayloadEntity(AEVec2 pos);
	~PayloadEntity() override;
	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void Render() override;
	void OnCollide(GameObjectEntity* go) override;
	bool GetBossActivated();
	void SetBossActivation(bool activated);
	AEVec2 GetBossRoomCenter();
	// Virtual methods to Enemy Base class
	void OnIdle(const f32& dt) override;
	void OnPatrol(const f32& dt) override;
	void OnChase(const f32& dt) override;
	void OnStun(const f32& dt) override;
	void OnDead(const f32& dt) override;
};



#endif