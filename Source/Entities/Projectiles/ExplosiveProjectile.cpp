#include "ExplosiveProjectile.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Entities/Enemies/EnemyEntity.hpp"
#include "../../Entities/TriggerEntities/ExplosionEntity.hpp"


ExplosiveProjectile::ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own, bool lerp)
    : BaseProjectile(pos, dir, speed, dmg, own)
{
    sprite = AssetManager::GetSpriteSheet(ASSET_PLAYER_SPRITE, 1, 1);
}

void ExplosiveProjectile::Update(const f32& dt)
{
    BaseProjectile::Update(dt);
}

void ExplosiveProjectile::OnHit(GameObjectEntity* other)
{
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
    OnExpire();
}

void ExplosiveProjectile::OnExpire()
{
    ExplosionEntity* explosion = new ExplosionEntity(this->position ,owner, damage);
    SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(explosion);
    BaseProjectile::OnExpire();

}

