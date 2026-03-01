#pragma once
#ifndef __ENEMYENTITY_H
#define __ENEMYENTITY_H
#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"
// Enemy Base class
class EnemyEntity : public GameObjectEntity {
protected:
	enum struct FSM {
		IDLE, PATROL, CHASE, STUN, DEAD, TOTAL
	} state;
	AEVec2 dir;
	f32 animationTimer, animationFrame, stateTimer;
	int currentRow, currentCol;
	SpriteSheet* sprite;
	// Helper function for switching the state.
	void SwitchState(FSM newState, f32 timeInNewState); 
public:
	EnemyEntity(AEVec2 pos, AEVec2 dir = { 0.0f,0.0f });
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
};
#endif