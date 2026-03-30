#include "SpikeProjectile.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"


SpikeProjectile::SpikeProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own) : BaseProjectile(pos, dir, speed, dmg, own)
{
    //Adjust sprite to spike and spike image is added
    sprite = AssetManager::GetSpriteSheet(ASSET_SPIKE_IMAGE, 1, 1); // single-frame bullet
    lifetime = 8.0f;
}


void SpikeProjectile::PreUpdate(const f32& dt) {
    BaseProjectile::PreUpdate(dt);
    if (!isActive)
    {
        return;
    }
    
}
