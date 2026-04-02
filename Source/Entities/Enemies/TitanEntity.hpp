/*!
* @file TitanEntity.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief Implementation of the TitanEntity boss class. This class defines
*        the behavior of a boss enemy including movement, state transitions,
*        attack patterns, projectile spawning, and death handling.
*/
#pragma once
#ifndef __TITANENTITY_H
#define __TITANENTITY_H
#include "BossEntity.hpp"
#include "../../Utils/Constant.hpp"
/*!
* @brief TitanEntity class that represents a boss enemy with state-driven behavior.
*        Handles movement, player chasing, attack patterns, projectile spawning,
*        and death logic. The Titan scales its aggression and attack intensity
*        based on its remaining health.
*/
class TitanEntity : public BossEntity {
protected:
	GameObjectEntity* ground;
	float shoot_cooldown{ BOSS1SHOOTCOOLDOWN };
	float shoot_timer;
public:
	/*!
	* @brief Constructs the TitanEntity and initializes its attributes such as
	*        sprite data, attack parameters, and projectile configuration
	* @param pos - Initial position of the Titan boss
	* @return None
	*/
	TitanEntity(AEVec2 pos);
	/*!
	* @brief Destructor for TitanEntity
	* @return None
	*/
	~TitanEntity() override;
	/*!
	* @brief Updates the Titan boss logic including cooldown timers and base behavior
	* @param dt - Delta time since last frame
	* @return None
	*/
	void Update(const f32& dt) override;
	/*!
	* @brief Handles post-update logic such as animation state selection and sprite flipping
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
	* @brief Handles patrol state behavior and transitions to chase after timer expires
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnPatrol(const f32& dt) override;
	/*!
	* @brief Handles chase behavior by jumping toward the player and transitioning to stun
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnChase(const f32& dt) override;
	/*!
	* @brief Handles stun state behavior including spawning projectiles and camera shake
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