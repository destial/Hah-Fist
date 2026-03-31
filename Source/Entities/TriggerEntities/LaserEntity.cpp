/*!
* @file LaserEntity.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 15 March 2026
* @course CSD1451
* @brief Declaration of the LaserEntity class, which represents a laser attack in the game.
*        This entity applies damage in a straight line and tracks its owner to prevent
*        friendly fire. It ensures entities are only damaged once per laser.
*/

#include "LaserEntity.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../PlayerEntity.hpp"
#include "../Enemies/EnemyEntity.hpp"
#include "../../Managers/SceneManager.hpp"
/*!
* @brief Constructs a LaserEntity at a given position with an owner and damage value
* @param pos - Position where the laser spawns
* @param _owner - Entity that spawned the laser
* @param _damage - Damage dealt by the laser
* @return None
*/
LaserEntity::LaserEntity(AEVec2 pos, GameObjectEntity* go, f32 dmg)
: GameObjectEntity{ pos,  1.f, CollisionShape::AABB, PhysicsType::TRIGGER },
    owner{ go }, damage{ dmg } {
	InitializeAnimatedSpriteData(ASSET_LASER_SPRITE, ASSET_LASER_SPRITE_ROWS, ASSET_LASER_SPRITE_COLUMNS, ASSET_LASER_SPRITE_SCALE);
    scale = { 6.0f,50.0f };
    position.x -= scale.y * 0.5f;
    rotation = -PI/2;
}
/*!
* @brief Destructor for LaserEntity
* @return None
*/
LaserEntity::~LaserEntity() {} // Empty dtor
/*!
* @brief Updates the laser entity each frame
* @param dt - Delta time since last frame
* @return None
*/
void LaserEntity::PostUpdate(const f32& dt) {
    if (currentCol == ASSET_LASER_SPRITE_COLUMNS - 1) {
        isActive = false;
        SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
    }
    GameObjectEntity::PostUpdate(dt);
}
/*!
* @brief Handles collision with another entity and applies damage if applicable
* @param other - Pointer to the entity collided with
* @return None
*/
void LaserEntity::OnCollide(GameObjectEntity* other) {
    if (!other || other == this || other == owner)
        return;

    if (!other->isActive)
        return;

    if (other->entity_type == EntityType::NONE)
        return;
    
    if (currentCol == 0) {
        if (previouslydamaged != other) {
            other->health -= damage;
            previouslydamaged = other;
        }

        if (other->entity_type == EntityType::ENEMY) {
            EnemyEntity* e = dynamic_cast<EnemyEntity*>(other);
            if (e) {
                e->OnHit();
                if (e->health < 0) {
                    e->SwitchState(EnemyEntity::FSM::DEAD);
                }
            }
        }
        else if (other->entity_type == EntityType::BREAKABLE_STATIC) {
            // Empty body
        }
    }
}
