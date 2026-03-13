#include "ExplosiveProjectile.hpp"
#include "../../Utils/AEOverload.hpp"

ExplosiveProjectile::ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own) : BaseProjectile(pos, dir, speed, dmg, own)
{
    this->frictionMultiplier = 5.f;
    lifetime = 3.0f;
}


void ExplosiveProjectile::PreUpdate(const f32& dt) {
    BaseProjectile::PreUpdate(dt);
    if (!isActive)
    {
        return;
    }
    /*if (slowStartTime > 0)
    {
        slowStartTime -= dt;
        if (slowStartTime <= 0)
        {
            velocity *= 15;
        }
    }
    else
    {
        pBody->gravityScale = AEClamp(pBody->gravityScale + dt * 0.1, 0.2f, 0.98f);
    }*/

    /*if (AEVec2Length(&velocity) < 20.0f)
    {
        velocity += velocity * dt * speedRamp;
        speedRamp += dt;
    }*/
    
}
