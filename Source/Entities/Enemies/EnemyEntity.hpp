#pragma once
#ifndef __ENEMYENTITY_H
#define __ENEMYENTITY_H
#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"
// Enemy Base class
class EnemyEntity : public GameObjectEntity {
protected:
	AEVec2 dir;
	f32 speed;
	f32 animationTimer, animationFrame, stateTimer;
	int currentRow, currentCol;
	SpriteSheet* sprite;
public:
	enum struct FSM {
		IDLE, PATROL, CHASE, STUN, DEAD, TOTAL
	} state;
	EnemyEntity(AEVec2 pos, AEVec2 dir = { 0.0f,0.0f }, f32 speed = 10.f);
	virtual ~EnemyEntity();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void OnCollide(GameObjectEntity* go);

	// Virtual methods to Enemy Base class
	virtual void OnIdle(const f32& dt);
	virtual void OnPatrol(const f32& dt);
	virtual void OnChase(const f32& dt);
	virtual void OnStun(const f32& dt);
	virtual void OnDead(const f32& dt);

	// Helper function for switching the state.
	void SwitchState(FSM newState, f32 timeInNewState = 0.0f);
};
#endif