/*!
* @file StaticEntity.hpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 13 March 2026
* @course CSD1451
* @brief This source file contains the definition of a StaticEntity base class for all StaticEntities in the game.
*/
#include "StaticEntity.hpp"
/*!
* @brief StaticEntity constructor that initialises the static_type, position, mass, collisionshape, and physicstype which will be defaulted to static.
* @param static_type - Type of static entity
* @param pos - Position of the static object
* @param mass - Mass of the static object to be used for physics calculations
* @param shape - Type of collision it is, AABB , OBB , etc..
* @param go_type - PhysicsType
*/
StaticEntity::StaticEntity(STATIC_TYPE static_type, AEVec2 pos, f32 mass, CollisionShape shape, PhysicsType go_type) 
: GameObjectEntity{ pos, mass, shape, go_type }, static_type{ static_type } {
	image = AssetManager::GetTexture(ASSET_STATIC_PLATFORM_IMAGE);
}

StaticEntity::~StaticEntity() {} // Empty dtor

/*!
* @brief Update function to update the entity
* @param dt - Time between frame
*/
void StaticEntity::Update(const f32& dt) {
	BaseEntity::Update(dt);
}

/*!
* @brief PostUpdate function uses parent's postupdate
* @param dt - Time between frame
*/
void StaticEntity::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);
}

/*!
* @brief OnCollide - Collision response
* @param go - Information of GameObject it is colliding with
*/
void StaticEntity::OnCollide(GameObjectEntity*) {
	// Empty body
}

/*!
* @brief GetStaticType - Gets the type of this static object
* @return STATIC_TYPE
*/
StaticEntity::STATIC_TYPE StaticEntity::GetStaticType() const {
	return static_type;
}
