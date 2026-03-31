/*!
* @file LaserEntity.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 15 March 2026
* @course CSD1451
* @brief Declaration of the LaserEntity class, which represents a laser attack in the game.
*        This entity applies damage in a straight line and tracks its owner to prevent
*        friendly fire. It ensures entities are only damaged once per laser.
*/

#pragma once
#ifndef __LASER_ENTITY_H
#define __LASER_ENTITY_H
#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"
/*!
* @brief LaserEntity class that handles straight-line damage from a laser attack.
*        Tracks its owner and prevents multiple damage applications to the same entity.
*/
class LaserEntity : public GameObjectEntity {
protected:
	GameObjectEntity* owner;
	GameObjectEntity* previouslydamaged{ nullptr };
	f32 damage;
public:
	/*!
	* @brief Constructs a LaserEntity at a given position with an owner and damage value
	* @param pos - Position where the laser spawns
	* @param _owner - Entity that spawned the laser
	* @param _damage - Damage dealt by the laser
	* @return None
	*/
	LaserEntity(AEVec2 pos, GameObjectEntity* _owner, f32 _damage);
	/*!
	* @brief Destructor for LaserEntity
	* @return None
	*/
	virtual ~LaserEntity();
	/*!
	* @brief Updates the laser entity each frame
	* @param dt - Delta time since last frame
	* @return None
	*/
	virtual void PostUpdate(const f32& dt);
	/*!
	* @brief Handles collision with another entity and applies damage if applicable
	* @param other - Pointer to the entity collided with
	* @return None
	*/
	virtual void OnCollide(GameObjectEntity* other);
};

#endif