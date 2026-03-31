#pragma once
#ifndef __BASEPROJECTILE_H
#define __BASEPROJECTILE_H

#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"

class BaseProjectile : public GameObjectEntity {
protected:
    f32 lifetime{ 5.f };
    f32 age{ 0.f };
    AEVec2 direction;
    f32 speed;
    f32 animationTimer, animationFrame;
    int currentRow, currentCol;
    SpriteSheet* sprite;
    GameObjectEntity* owner;
public:
    BaseProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 damage, GameObjectEntity* _owner);
    virtual ~BaseProjectile() = default;

    virtual void PreUpdate(const f32& dt);
    virtual void Update(const f32& dt);
    virtual void Render();
    virtual void OnCollide(GameObjectEntity* other);
    virtual void OnHit(GameObjectEntity* other);
    bool TryChangeOwnership(GameObjectEntity* newOwner);

protected:
    virtual void OnExpire();                      
};

#endif