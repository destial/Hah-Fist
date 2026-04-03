/*!
* @file BaseProjectile.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 3 March 2026
* @course CSD1451
* @brief Declaration of the BaseProjectile class which represents a generic
*        projectile entity in the game. This class handles movement,
*        lifetime tracking, animation, collision handling, and ownership.
*        It serves as a base class for all projectile types.
*/
#include "BaseProjectile.hpp"
#include <cmath>
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Entities/Enemies/EnemyEntity.hpp"
#include "../../Entities/TriggerEntities/ExplosionEntity.hpp"

/*!
* @brief Constructs a BaseProjectile and initializes its properties such as
*        movement, sprite, physics settings, and ownership
* @param pos - Initial position of the projectile
* @param dir - Direction vector of movement
* @param speed - Movement speed of the projectile
* @param dmg - Damage dealt upon collision
* @param _owner - Pointer to the entity that owns this projectile
* @return None
*/
BaseProjectile::BaseProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* _owner)
: GameObjectEntity{ pos, 1.f, CollisionShape::AABB }, direction{ dir }, speed{ speed } {
    damage = dmg;
    sprite = AssetManager::GetSpriteSheet(ASSET_PROJECTILE_IMAGE, 1, 1); // single-frame bullet
    mesh = nullptr;

    //Projectiles with animations
    animation_timer = 0.f;
    animation_frame = 1.f / (1.f * 1.f);
    current_row = current_col = 0;

    scale = { 2.f * (static_cast<f32>(sprite->image->width) / sprite->image->height), 2.f };
    layer = RenderLayer::ENTITY;
    pBody->gravity_scale = 0.f;
    this->frictionMultiplier = 0.f;
    is_active = true;
    velocity.x = dir.x * speed;
    velocity.y = dir.y * speed;
    health = 1;
    owner = _owner;

    entity_type = EntityType::PROJECTILE;
    go_type = PhysicsType::TRIGGER;
}

/*!
* @brief Handles pre-update logic such as aligning projectile rotation with velocity
* @param dt - Delta time since last frame
* @return None
*/
void BaseProjectile::PreUpdate(const f32& dt) {
    GameObjectEntity::PreUpdate(dt);
    AEVec2 right = { 1.f, 0 };
    rotation = AEVec2AngleCCW(&right, &velocity);
}
/*!
* @brief Updates projectile state including lifetime tracking and expiration
* @param dt - Delta time since last frame
* @return None
*/
void BaseProjectile::Update(const f32& dt) {
    if (!is_active) 
        return;
    
    GameObjectEntity::Update(dt);
 
    age += dt;
    if (age >= lifetime){
        OnExpire();
    }
}
/*!
* @brief Renders the projectile sprite and any additional debug or base visuals
* @return None
*/
void BaseProjectile::Render() {
    sprite->Render(transform, color, current_row, current_col);
    GameObjectEntity::Render();
}
/*!
* @brief Handles collision logic and filters out invalid collision cases
* @param other - Pointer to the object collided with
* @return None
*/
void BaseProjectile::OnCollide(GameObjectEntity* other) {
    if (!other || other == this || other == owner)
        return;

    if (!other->is_active)
        return;

    if (other->entity_type == EntityType::NONE)
        return;
    if (BaseProjectile* bp = dynamic_cast<BaseProjectile*>(other))
    {
        if (bp->owner == owner)
        {
            return;
        }
    }

    if (dynamic_cast<ExplosionEntity*>(other))
        return;

    OnHit(other);
}
/*!
* @brief Applies damage and triggers effects when the projectile hits another object
* @param other - Pointer to the object hit
* @return None
*/
void BaseProjectile::OnHit(GameObjectEntity* other) {
    // Apply direct damage
    other->health -= damage;
    
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
    OnExpire();
}
/*!
* @brief Handles projectile expiration by deactivating and removing it from the scene
* @return None
*/
void BaseProjectile::OnExpire() {
    is_active = false;
    SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}

/*!
* @brief Attempts to change the owner of the projectile
* @param newOwner - Pointer to the new owner entity
* @return True if ownership change is successful, false otherwise
*/
bool BaseProjectile::TryChangeOwnership(GameObjectEntity* newOwner) {
    if (owner == newOwner)
    {
        return false;
    }
    owner = newOwner;
    return true;
}