/*!
* @file ProjectileEntity.hpp
* @author Brandon Koh Shao Hui (brandonshaohui.koh@digipen.edu)
* @date 14 March 2026
* @course CSD1451
* @brief This source file contains the declaration for class ProjectileEntity as well as its member functions
*		 which contains the behaviour of ProjectileEntity which is the archer in the game.
*/
#pragma once
#ifndef __PROJECTILE_ENTITY_H
#define __PROJECTILE_ENTITY_H

#define INITIAL_SPEED 10.f
#include "EnemyEntity.hpp"
/*!
* @brief ProjectileEntity class that handles the archer in-game
*/
class ProjectileEntity : public EnemyEntity {
private:
public:
	/*!
	* @brief Constructor of ProjectileEntity which initialises its data members position & speed
	* @param pos - Position of the ProjectileEntity
	* @param speed - Speed of the ProjectileEntity
	*/
	ProjectileEntity(AEVec2 pos, f32 speed = INITIAL_SPEED);

	/*!
	* @brief Destructor of the ProjectileEntity
	*/
	~ProjectileEntity() override;

	/*!
	* @brief Updates which way the sprites faces and its fsm behaviour
	* @param dt - Time between frame
	*/
	void Update(const f32& dt) override;

	/*!
	* @brief PostUpdate
	* @param dt - Time between frame
	*/
	void PostUpdate(const f32& dt) override;

	/*!
	* @brief OnCollide function which contains the collision response of the ProjectileEntity
	* @param go - GameObjectEntity it is colliding
	*/
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class

	/*!
	* @brief OnIdle contains the idle behaviour of the ProjectileEntity
	* @param dt - Time between each frame
	*/
	void OnIdle(const f32& dt) override;

	/*!
	* @brief OnPatrol contains the patrol behaviour of the ProjectileEntity
	* @param dt - Time between frame
	*/
	void OnPatrol(const f32& dt) override;

	/*!
	* @brief OnChase contains the chase behaviour of the ProjectileEntity
	* @param dt - Time between each frame
	*/
	void OnChase(const f32& dt) override;

	/*!
	* @brief OnAttack contains the attack behaviour of the ProjectileEntity
	* @param dt - Time between each frame
	*/
	void OnAttack(const f32& dt) override;

	/*!
	* @brief OnStun contains the stun behaviour of the ProjectileEntity
	* @param dt - Time between each frame
	*/
	void OnStun(const f32& dt) override;

	/*!
	* @brief OnDead contains the dead behaviour of the ProjectileEntity
	* @param dt - Time between each frame
	*/
	void OnDead() override;
};
#endif