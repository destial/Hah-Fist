/*!
* @file PayloadEntity.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief Implementation of the PayloadEntity boss class. This class defines
*        behavior for a boss enemy that uses a multi-phase attack system,
*        including jumping, platform spawning, and projectile attacks.
*        The boss scales its attack intensity based on remaining health.
*/
#pragma once
#ifndef __PAYLOADENTITY_H
#define __PAYLOADENTITY_H
#include "BossEntity.hpp"
#include "../../Utils/Constant.hpp"
/*!
* @brief PayloadEntity class that represents a boss enemy with a multi-phase
*        attack system driven by an internal state machine. The boss performs
*        jump-based movement, spawns temporary platforms, and launches projectile
*        attacks. Its attack intensity and behavior scale based on remaining health.
*/
class PayloadEntity : public BossEntity {
public:
	enum struct INNERFSM {
		JUMP,LAND,ATTACK, TOTAL
	};
protected:
	INNERFSM inner_state;
	GameObjectEntity* ground;
	float land_cooldown{ BOSS2SHOOTCOOLDOWN };
	float land_timer;
	float attack_range;
	int base_projectiles, extra_projectiles;
public:
	/*!
	* @brief Constructs the PayloadEntity and initializes its attributes such as
	*        sprite data, attack parameters, and internal state machine
	* @param pos - Initial position of the Payload boss
	* @return None
	*/
	PayloadEntity(AEVec2 pos);
	/*!
	* @brief Destructor for PayloadEntity
	* @return None
	*/
	~PayloadEntity() override;
	/*!
	* @brief Handles post-update logic such as animation selection and sprite orientation
	* @param dt - Delta time since last frame
	* @return None
	*/
	void PostUpdate(const f32& dt) override;
	/*!
	* @brief Handles idle state behavior and transitions to chase when activated
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnIdle(const f32& dt) override;
	/*!
	* @brief Handles chase behavior using an internal state machine for jump,
	*        landing, and attack phases
	* @param dt - Delta time since last frame
	* @return None
	*/
	void OnChase(const f32& dt) override;
	/*!
	* @brief Handles stun state behavior and transitions back to chase when timer expires
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnStun(const f32& dt) override;
	/*!
	* @brief Handles boss death by triggering win condition and removing the entity
	* @return None
	*/
	void OnDead() override;
};



#endif