/*!
* @file ExplosiveProjectile.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 13 March 2026
* @course CSD1451
* @brief ExplosiveProjectile class that represents a projectile which triggers
*        an explosion upon impact or expiration. Extends BaseProjectile to add
*        area-of-effect damage behavior.
*/
#include "ExplosiveProjectile.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Entities/Enemies/EnemyEntity.hpp"
#include "../../Entities/TriggerEntities/ExplosionEntity.hpp"
/*!
* @brief Constructs an ExplosiveProjectile with optional lerp behavior
* @param pos - Initial position of the projectile
* @param dir - Direction vector of movement
* @param speed - Movement speed of the projectile
* @param dmg - Damage dealt by the explosion
* @param own - Pointer to the owning entity
* @param lerp - Whether the projectile uses interpolated movement (optional)
* @return None
*/
ExplosiveProjectile::ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own, bool)
: BaseProjectile{ pos, dir, speed, dmg, own } {
    sprite = AssetManager::GetSpriteSheet(ASSET_EXPLOSIVE_PROJECTILE_IMAGE, 1, 1);
}
/*!
* @brief Handles collision by triggering explosion effects instead of standard hit logic
* @param other - Pointer to the object collided with
* @return None
*/
ExplosiveProjectile::~ExplosiveProjectile() {} // Empty dtor

void ExplosiveProjectile::OnHit(GameObjectEntity* other) {
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
* @brief Handles projectile expiration by triggering an explosion
* @return None
*/
void ExplosiveProjectile::OnExpire() {
    ExplosionEntity* explosion = new ExplosionEntity{ position ,owner, damage };
    SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(explosion);
    BaseProjectile::OnExpire();
}

