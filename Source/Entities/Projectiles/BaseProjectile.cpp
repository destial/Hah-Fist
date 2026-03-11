#include "BaseProjectile.hpp"
#include <cmath>
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Entities/Enemies/EnemyEntity.hpp"

BaseProjectile::BaseProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own)
: GameObjectEntity{ pos, 1.f, CollisionShape::AABB }, direction{ dir }, speed{ speed } {
    damage = dmg;
    sprite = AssetManager::GetSpriteSheet("Assets/projectile.png", 1, 1); // single-frame bullet
    mesh = nullptr;

    //Might not need any of this
    animationTimer = 0.f;
    animationFrame = 1.f / (1.f * 1.f);
    currentRow = currentCol = 0;
    //try removing

    scale = { 2.f * (static_cast<f32>(sprite->image->width) / sprite->image->height), 2.f };
    layer = RenderLayer::ENTITY;
    pBody->gravityScale = 0.f;
    this->frictionMultiplier = 0.f;
    isActive = true;
    velocity.x = dir.x * speed;
    velocity.y = dir.y * speed;
    health = 1;
    owner = own;

    entity_type = EntityType::PROJECTILE;
}


void BaseProjectile::PreUpdate(const f32& dt) {
    GameObjectEntity::PreUpdate(dt);
    AEVec2 right = { 1.f, 0 };
    rotation = AEVec2AngleCCW(&right, &velocity);
}

void BaseProjectile::Update(const f32& dt) {

    if (!isActive) {
        return;
    }
    GameObjectEntity::Update(dt);
 
    age += dt;
    if (age >= lifetime)
    {
        isActive = false;
        OnExpire();
        SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
    }

}
void BaseProjectile::PostUpdate(const f32& dt)
{
    GameObjectEntity::PostUpdate(dt);
}

void BaseProjectile::Render()
{
    sprite->Render(transform, color, currentRow, currentCol);
    GameObjectEntity::Render();
}

void BaseProjectile::OnCollide(GameObjectEntity* other)
{
    if (!other || other == this || other == owner)
        return;

    if (!other->isActive)
        return;

    OnHit(other);
}
void BaseProjectile::OnHit(GameObjectEntity* other)
{
    // Apply direct damage
    other->health -= damage;
    if (other->health < 0)
    {
        if (EnemyEntity* e = dynamic_cast<EnemyEntity*>(other))
            e->SwitchState(EnemyEntity::FSM::DEAD);
        //else
        //    SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(other);
        
    }

    // Destroy projectile
    isActive = false;
    OnExpire();
    SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
void BaseProjectile::OnExpire()
{
    // Default: just disappear
}
