#include "LaserEntity.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../PlayerEntity.hpp"
#include "../Enemies/EnemyEntity.hpp"
#include "../../Managers/SceneManager.hpp"

LaserEntity::LaserEntity(AEVec2 pos, GameObjectEntity* _owner, f32 _damage) : GameObjectEntity(pos, 1.f, CollisionShape::AABB, PhysicsType::TRIGGER)
{
	InitializeAnimatedSpriteData(ASSET_COIN_SPRITE, ASSET_COIN_SPRITE_ROWS, ASSET_COIN_SPRITE_COLUMNS, ASSET_COIN_SPRITE_SCALE);
    owner = _owner;
    damage = _damage;
    scale = { 50.0f,3.0f };
    position.x -= scale.x * 0.5f;
}

LaserEntity::~LaserEntity()
{ 

}

void LaserEntity::Update(const f32& dt) {
    GameObjectEntity::Update(dt);
}
void LaserEntity::PostUpdate(const f32& dt) {
    if (currentCol == ASSET_COIN_SPRITE_COLUMNS - 1)
    {
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
    {
        return;
    }
    if (currentCol == 0)
    {
        if (previouslydamaged != other)
        {
            other->health -= damage;
            previouslydamaged = other;
        }

        if (other->entity_type == EntityType::ENEMY)
        {
            EnemyEntity* e = dynamic_cast<EnemyEntity*>(other);
            if (e) {
                e->OnHit();
                if (e->health < 0) {
                    e->SwitchState(EnemyEntity::FSM::DEAD);
                }
            }
        }
        else if (other->entity_type == EntityType::BREAKABLE_STATIC)
        {

        }
    }
}
