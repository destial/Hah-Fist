/*!
* @file EnemyEntity.hpp
* @author Brandon Koh Shao Hui (brandonshaohui.koh@digipen.edu)
* @date 21 January 2026
* @course CSD1451
* @brief This source file contains the declaration of class EnemyEntity as well as its member
*		 functions which is in charge of the behaviours of the Enemy.
*/
#pragma once
#ifndef __ENEMYENTITY_H
#define __ENEMYENTITY_H
#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"
/*!
* @brief EnemyEntity base class that handles the generic behaviours of the enemies.
*/
class EnemyEntity : public GameObjectEntity {
public:
	enum struct FSM {
		IDLE, PATROL, CHASE,ATTACK, STUN, DEAD, TOTAL
	};
protected:
	FSM state;
	AEVec2 dir;
	f32 speed;
	f32 stateTimer;
public:
	/*!
	* @brief Constructor of the EnemyEntity class which initialises the position, direction, speed.
	* @param pos - Position of the EnemyEntity
	* @param dir - Direction of the EnemyEntity
	* @param speed - Speed of the EnemyEntity
	* @param inherited - Status of EnemyEntity's inheritance
	*/
	EnemyEntity(AEVec2 pos, AEVec2 dir = { 0.0f,0.0f }, f32 speed = 10.f, bool inherited = false);

	/*!
	* @brief Virtual Destructor of the EnemyEntity. Uses the destructor synthesised by compiler.
	*/
	virtual ~EnemyEntity() = default;

	/*!
	* @brief PreUpdate function
	* @param dt - Time between each frame
	*/
	virtual void PreUpdate(const f32& dt);

	/*!
	* @brief Update function
	* @param dt - Time between each frame
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief PostUpdate function
	* @param dt - Time between each frame
	*/
	virtual void PostUpdate(const f32& dt);

	// Virtual methods to Enemy Base class

	/*!
	* @brief OnHit function. This contains the collision response of the EnemyEntity
	*/
	virtual void OnHit();

	/*!
	* @brief OnIdle function. This contains the idle behaviour of the EnemyEntity
	*/
	virtual void OnIdle(const f32& dt);

	/*!
	* @brief OnPatrol function. This contains the Patrol behaviour of the EnemyEntity
	*/
	virtual void OnPatrol(const f32& dt);

	/*!
	* @brief OnChase function. This contains the Chasing behaviour of the EnemyEntity
	*/
	virtual void OnChase(const f32& dt);

	/*!
	* @brief OnAttack function. This contains the Attack behaviour of the EnemyEntity
	*/
	virtual void OnAttack(const f32& dt);

	/*!
	* @brief OnStun function. This contains the Stun behaviour of the EnemyEntity
	*/
	virtual void OnStun(const f32& dt);

	/*!
	* @brief OnDead function. This contains the Dead behaviour of the EnemyEntity
	*/
	virtual void OnDead();

	/*!
	* @brief Helper function for switching the FSM states of the Enemy
	*/
	void SwitchState(FSM newState, f32 timeInNewState = 0.0f);

	/*!
	* @brief Fetches the current state of the enemy.
	* @return FSM
	*/
	FSM GetCurrentState() const;

	/*!
	* @brief Helper function for switching the direction of the enemy.
	*/
	void FlipDir();
	
};
#endif