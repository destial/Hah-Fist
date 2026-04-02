/*!
* @file StaticEntity.hpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 13 March 2026
* @course CSD1451
* @brief This source file contains the declaration of a StaticEntity base class for all StaticEntities in the game.
*/
#pragma once
#ifndef __STATIC_ENTITY_H
#define __STATIC_ENTITY_H
#include "GameObjectEntity.hpp"
/*!
* @brief StaticEntity class that handles the behaviours of static entities as well as its collision response
*/
class StaticEntity : public GameObjectEntity {
public:
	enum class STATIC_TYPE {
		TYPE_WALL,
		TYPE_PLATFORM
	}static_type;
public:

	/*!
	* @brief StaticEntity constructor that initialises the static_type, position, mass, collisionshape, and physicstype which will be defaulted to static.
	* @param static_type - Type of static entity
	* @param pos - Position of the static object
	* @param mass - Mass of the static object to be used for physics calculations
	* @param shape - Type of collision it is, AABB , OBB , etc..
	* @param go_type - PhysicsType
	*/
	StaticEntity(STATIC_TYPE static_type,AEVec2 pos = { 0.f,0.f }, f32 mass = 1.f, CollisionShape shape = CollisionShape::AABB, PhysicsType go_type = PhysicsType::STATIC); // Ctor
	StaticEntity(StaticEntity const&) = delete; // Remove copy ctor
	StaticEntity& operator=(StaticEntity const&) = delete; // Remove copy assignment

	/*!
	* @brief Destructor of the StaticEntity class
	*/
	virtual ~StaticEntity(); // Dtor

	/*!
	* @brief Update function to update the entity
	* @param dt - Time between frame
	*/
	void Update(const f32& dt) override;

	/*!
	* @brief PostUpdate function uses parent's postupdate
	* @param dt - Time between frame
	*/
	void PostUpdate(const f32& dt) override;

	/*!
	* @brief OnCollide - Collision response
	* @param go - Information of GameObject it is colliding with
	*/
	void OnCollide(GameObjectEntity* go) override;

	/*!
	* @brief GetStaticType - Gets the type of this static object
	* @return STATIC_TYPE
	*/
	STATIC_TYPE GetStaticType() const;

};
#endif