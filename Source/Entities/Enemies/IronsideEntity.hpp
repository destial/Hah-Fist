#pragma once
#ifndef __IRONSIDEENTITY_H
#define __IRONSIDEENTITY_H
#include "EnemyEntity.hpp"
class IronsideEntity : public EnemyEntity {
public:
	enum struct INNERFSM {
		MOVE,MOVING,SPAWNPLATFORM1, SPAWNPLATFORM2, SHOOTPROJECTILE, LASER, TOTAL
	}; 
	enum LANE {
		LANE1 = 1, LANE2 = 2, LANE3 = 3, TOTAL
	};
protected:
	INNERFSM innerState;
	LANE currLane;
	GameObjectEntity* ground;
	float shootCooldown{ 3.f };
	float shootTimer;
	bool bossActivated;
	float bossRoomX, bossRoomY, attackRange, jumpX, jumpY;
	int baseProjectiles, extraProjectiles;
	int nextLanetospawn;
	LANE lanetogoto;     
	float dirtogo;
	float targetY;
	
public:
	IronsideEntity(AEVec2 pos);
	~IronsideEntity() override;
	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void Render() override;
	void OnCollide(GameObjectEntity* go) override;
	LANE GetRandomSpawnLane(LANE bossLane);
	// Virtual methods to Enemy Base class
	void OnIdle(const f32& dt) override;
	void OnPatrol(const f32& dt) override;
	void OnChase(const f32& dt) override;
	void OnStun(const f32& dt) override;
	void OnDead(const f32& dt) override;
};



#endif