/*!
* @file TrooperEntity.hpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 24 February 2026
* @course CSD1451
* @brief This source file contians the declaration of class TrooperEntity as well as its member functions
*		 which is the slime enemy in the game.
*/
#pragma once
#ifndef __TROOPERENTITY_H
#define __TROOPERENTITY_H
#include "EnemyEntity.hpp"
/*!
* @brief TrooperEntity class
*/
class TrooperEntity : public EnemyEntity {
public:
	/*!
	* @brief Constructor of the TrooperEntity class. Takes in the position & speed and intialises it.
	* @param pos - Position of the trooper
	* @param speed - Speed of the trooper
	*/
	TrooperEntity(AEVec2 pos, f32 speed = 10.f);

	/*!
	* @brief Destructor of the Trooper Entity
	*/
	~TrooperEntity() override;

	/*!
	* @brief PostUpdate of TrooperEntity
	*/
	void PostUpdate(const f32& dt) override;

	/*!
	* @brief OnCollide function which contains the collision response of the TrooperEntity
	*/
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class

	/*!
	* @brief OnIdle override of EnemyEntity class. This contains the Idle behaviour of the Trooper(Slime)
	* @param dt - Time between every frame
	*/
	void OnIdle(const f32& dt) override;

	/*!
	* @brief OnDead override of EnemyEntity class. This contains the Dead behaviour of the Trooper(Slime) when it dies.
	*/
	void OnDead() override;
};
#endif