#pragma once
#ifndef __IRONSIDEENTITY_H
#define __IRONSIDEENTITY_H
#include "EnemyEntity.hpp"
class IronsideEntity : public EnemyEntity {
protected:
	GameObjectEntity* ground;
	float shootCooldown{ 3.f };
	float shootTimer;
	bool bossActivated;
	float bossRoomX, bossRoomY, attackRange, jumpX, jumpY;
	int baseProjectiles, extraProjectiles;
public:
	IronsideEntity(AEVec2 pos);
	~IronsideEntity() override;
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