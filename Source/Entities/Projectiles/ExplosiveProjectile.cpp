#include "ExplosiveProjectile.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Entities/Enemies/EnemyEntity.hpp"

ExplosiveProjectile::ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own, bool lerp)
    : BaseProjectile(pos, dir, speed, dmg, own)
{
    sprite = AssetManager::GetSpriteSheet(ASSET_PLAYER_SPRITE, 1, 1);
    lifetime = 3.f;
    explosionDamage = dmg;
    exploded = false;
}

void ExplosiveProjectile::Update(const f32& dt)
{
    BaseProjectile::Update(dt); // call parent (rotation, etc.)
    if (exploded)
    {
        slowStartTime -= dt;
        if (slowStartTime < 0.f)
        {
            Explode();
        }
    }
    
}

void ExplosiveProjectile::OnHit(GameObjectEntity* other)
{
    BaseProjectile::OnHit(other);
}

void ExplosiveProjectile::OnExpire()
{
    // can add particle effects or sound here
    scale.x = explosionRadius;
    scale.y = explosionRadius;
    exploded = true;
    velocity.x = 0;
    velocity.y = 0;

}

void ExplosiveProjectile::Explode()
{
    BaseProjectile::OnExpire();
}
