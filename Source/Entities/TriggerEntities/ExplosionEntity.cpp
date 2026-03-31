/*!
* @file ExplosionEntity.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 15 March 2026
* @course CSD1451
* @brief Declaration of the ExplosionEntity class, which represents an explosion
*        in the game. This entity deals area-of-effect damage to other entities
*        and tracks its owner to avoid friendly fire.
*/
#include "ExplosionEntity.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../PlayerEntity.hpp"
#include "../Enemies/EnemyEntity.hpp"
#include "../../Managers/SceneManager.hpp"
/*!
* @brief Constructs an ExplosionEntity at a given position with an owner and damage
* @param pos - Position where the explosion occurs
* @param owner - Entity that spawned the explosion
* @param damage - Damage dealt by the explosion
* @return None
*/
ExplosionEntity::ExplosionEntity(AEVec2 pos, GameObjectEntity* go, f32 dmg) 
: GameObjectEntity{ pos,  1.f, CollisionShape::AABB, PhysicsType::TRIGGER },
  owner{ go }, damage{ dmg } {
	InitializeAnimatedSpriteData(ASSET_EXPLOSION_SPRITE, ASSET_EXPLOSION_SPRITE_ROWS, ASSET_EXPLOSION_SPRITE_COLUMNS, ASSET_EXPLOSION_SPRITE_SCALE);
    scale = { 3.0f, 3.0f };
}
/*!
* @brief Destructor for ExplosionEntity
* @return None
*/
ExplosionEntity::~ExplosionEntity() {} // Empty dtor
/*!
* @brief Updates the explosion entity each frame
* @param dt - Delta time since last frame
* @return None
*/
void ExplosionEntity::PostUpdate(const f32& dt) {
    if (currentCol == ASSET_EXPLOSION_SPRITE_COLUMNS - 1) {
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
void ExplosionEntity::OnCollide(GameObjectEntity* other) {
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
            AEAudioPlay(AssetManager::GetAudio(ASSET_EXPLOSION_PICKUP_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
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