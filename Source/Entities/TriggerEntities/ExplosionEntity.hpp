/*!
* @file ExplosionEntity.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 15 March 2026
* @course CSD1451
* @brief Declaration of the ExplosionEntity class, which represents an explosion
*        in the game. This entity deals area-of-effect damage to other entities
*        and tracks its owner to avoid friendly fire.
*/
#pragma once
#ifndef __EXPLOSION_ENTITY_H
#define __EXPLOSION_ENTITY_H
#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"
/*!
* @brief ExplosionEntity class that handles area-of-effect damage when a projectile
*        or other explosive object detonates. It keeps track of its owner and
*        prevents multiple damage applications to the same entity.
*/
class ExplosionEntity : public GameObjectEntity {
protected:
	GameObjectEntity* owner;
	GameObjectEntity* previouslydamaged{nullptr};
	f32 damage;
public:
	/*!
	* @brief Constructs an ExplosionEntity at a given position with an owner and damage
	* @param pos - Position where the explosion occurs
	* @param owner - Entity that spawned the explosion
	* @param damage - Damage dealt by the explosion
	* @return None
	*/
	ExplosionEntity(AEVec2 pos, GameObjectEntity* owner, f32 damage);
	/*!
	* @brief Destructor for ExplosionEntity
	* @return None
	*/
	virtual ~ExplosionEntity();
	/*!
	* @brief Updates the explosion entity each frame
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