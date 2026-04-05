#include "MissileProjectile.hpp"
#include "../../Utils/AEOverload.hpp"

MissileProjectile::MissileProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own)
: BaseProjectile{ pos, dir, speed, dmg, own } {
    friction_multiplier = 5.f;
    lifetime = 3.0f;
}

MissileProjectile::~MissileProjectile() {} // Empty dtor

