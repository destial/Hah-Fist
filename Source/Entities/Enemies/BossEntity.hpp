/*!
* @file BossEntity.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 28 February 2026
* @course CSD1451
* @brief Implementation of the BossEntity base class. This class provides
*        core functionality for all boss types, including health management,
*        activation state, projectile attacks, and common state behaviors
*        (idle, patrol, chase, attack, and stun). It also serves as a
*        foundation for derived boss classes to extend unique mechanics.
*/

#pragma once
#ifndef __BOSSENTITY_H
#define __BOSSENTITY_H
#include "EnemyEntity.hpp"
/*!
* @brief Implementation of the BossEntity base class. This class provides
*        core functionality for all boss types, including health management,
*        activation state, projectile attacks, and common state behaviors
*        (idle, patrol, chase, attack, and stun). It also serves as a
*        foundation for derived boss classes to extend unique mechanics.
*/
class BossEntity : public EnemyEntity {
protected:
	GameObjectEntity* ground;
	bool boss_activated;
	AEVec2 boss_room_center;
public:
	BossEntity(AEVec2 pos);
	virtual ~BossEntity();

	//Default Boss functions
	/*!
	* @brief Spawns a projectile based on the boss's current health
	* @param healthRatio - Current health ratio of the boss (0.0 to 1.0)
	* @param Pos - Spawn position of the projectile
	* @param shootDir - Direction in which the projectile is fired
	*/
	void ShootProjectile(float healthRatio, AEVec2 Pos, AEVec2 shootDir);
	/*!
	* @brief Calculates a scaling factor based on the boss's remaining health
	* @return A value between 0 and 1 indicating low-health intensity
	*/
	float GetLowHealthFactor();
	/*!
	* @brief Gets whether the boss has been activated
	* @return True if the boss is activated, false otherwise
	*/
	bool GetBossActivated() const;
	/*!
	* @brief Sets the boss activation state
	* @param activated - Boolean to activate or deactivate the boss
	*/
	void SetBossActivation(bool activated);
	/*!
	* @brief Retrieves the center position of the boss room
	* @return AEVec2 representing the boss room center
	*/
	AEVec2 GetBossRoomCenter() const;
	// Virtual methods to Enemy Base class
	/*!
	* @brief Idle state behavior of the boss
	* @param dt - Delta time for frame update
	*/
	virtual void OnIdle(const f32& dt);
	/*!
	* @brief Patrol state behavior of the boss
	* @param dt - Delta time for frame update
	*/
	virtual void OnPatrol(const f32& dt);
	/*!
	* @brief Chase state behavior of the boss
	* @param dt - Delta time for frame update
	*/
	virtual void OnChase(const f32& dt);
	/*!
	* @brief Attack state behavior of the boss
	* @param dt - Delta time for frame update
	*/
	virtual void OnAttack(const f32& dt);
	/*!
	* @brief Stun state behavior of the boss
	* @param dt - Delta time for frame update
	*/
	virtual void OnStun(const f32& dt);
	/*!
	* @brief Handles boss death by removing it from the current scene
	*/
	virtual void OnDead();
};

#endif