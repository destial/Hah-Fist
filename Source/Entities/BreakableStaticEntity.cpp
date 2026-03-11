#include "BreakableStaticEntity.hpp"
#include "../Managers/SceneManager.hpp"

BreakableStaticEntity::BreakableStaticEntity(STATIC_TYPE static_type, AEVec2 pos, f32 mass, CollisionShape shape, PhysicsType go_type) : StaticEntity(static_type, pos, mass, shape, go_type)
{
	entity_type = EntityType::BREAKABLE_STATIC;
}

BreakableStaticEntity::~BreakableStaticEntity() {}

void BreakableStaticEntity::Update(const f32& dt)
{
	StaticEntity::Update(dt);
	if (health <= 0.f && isActive) {
		OnBroken();
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
	}
}

void BreakableStaticEntity::OnBroken()
{

}