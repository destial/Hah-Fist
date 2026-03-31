#include "LaserEntity.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../PlayerEntity.hpp"
#include "../Enemies/EnemyEntity.hpp"
#include "../../Managers/SceneManager.hpp"

LaserEntity::LaserEntity(AEVec2 pos, GameObjectEntity* go, f32 dmg)
: GameObjectEntity{ pos,  1.f, CollisionShape::AABB, PhysicsType::TRIGGER },
    owner{ go }, damage{ dmg } {
	InitializeAnimatedSpriteData(ASSET_LASER_SPRITE, ASSET_LASER_SPRITE_ROWS, ASSET_LASER_SPRITE_COLUMNS, ASSET_LASER_SPRITE_SCALE);
    scale = { 6.0f,50.0f };
    position.x -= scale.y * 0.5f;
    rotation = -PI/2;
}

LaserEntity::~LaserEntity() {} // Empty dtor

void LaserEntity::PostUpdate(const f32& dt) {
    if (currentCol == ASSET_LASER_SPRITE_COLUMNS - 1) {
        isActive = false;
        SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
    }
    GameObjectEntity::PostUpdate(dt);
}

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
