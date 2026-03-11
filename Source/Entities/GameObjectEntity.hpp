/*!
* @file GameObjectEntity.hpp
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 19 January 2026
* @course CSD1451
* @brief Declaration file for a gameobject entity that can collide
*/

#pragma once
#ifndef __GAMEOBJECTENTITY_H
#define __GAMEOBJECTENTITY_H
#include "BaseEntity.hpp"
#include "../Utils/PhysicsBody.hpp"

class GameObjectEntity : public BaseEntity {
public:
	PhysicsBody* pBody; // Physics body for this entity
	AEVec2 prev_position; // The position at the previous frame
	f32 frictionMultiplier{ 1.0 };
	bool isActive;
	f32 health;
	f32 max_health;
	f32 damage;
	enum struct CollisionShape : int {
		CIRCLE = 0, AABB, TOTAL
	} shape; // Collision shape for calculating collision checks

	enum struct PhysicsType : int {
		STATIC = 0, DYNAMIC, TRIGGER, TOTAL
	} go_type; // Physics type for calculating collision responses

	enum struct EntityType : int {
		NONE = 0, PLAYER, ENEMY, PROJECTILE, BREAKABLE_STATIC,
	} entity_type = EntityType::NONE;

	GameObjectEntity(); // Ctor
	GameObjectEntity(AEVec2 pos = { 0.f, 0.f }, f32 mass = 1.0f, CollisionShape type = CollisionShape::AABB, PhysicsType go_type = PhysicsType::DYNAMIC); // Ctor
	GameObjectEntity(GameObjectEntity const&) = delete; // Remove copy ctor
	GameObjectEntity& operator=(GameObjectEntity const&) = delete; // Remove copy assignment
	virtual ~GameObjectEntity(); // Dtor

	/*!
	* @brief Inherited: Pre-update the entity, before any input has been processed
	*/
	virtual void PreUpdate(const f32& dt);

	/*!
	* @brief Inherited: Update the entity after input has been processed
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Inherited: Post-update the entity after everything else has been processed
	*/
	virtual void PostUpdate(const f32& dt);

	/*!
	* @brief Inherited: Render the entity to the screen
	*/
	virtual void Render();

	/*!
	* @brief Called when this entity collides with another GameObject entity
	* @param go - The collided GameObject entity
	*/
	virtual void OnCollide(GameObjectEntity* go);

};
#endif