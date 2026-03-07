#include "StaticEntity.hpp"

StaticEntity::StaticEntity(STATIC_TYPE static_type, AEVec2 pos, f32 mass, CollisionShape shape, PhysicsType go_type) :static_type{ static_type }, GameObjectEntity(pos, mass, shape, go_type)
{
}

void StaticEntity::PreUpdate(const f32& dt)
{
	GameObjectEntity::PreUpdate(dt);
}

void StaticEntity::Update(const f32& dt)
{
	BaseEntity::Update(dt);
}

void StaticEntity::PostUpdate(const f32& dt)
{
	BaseEntity::PostUpdate(dt);
}

void StaticEntity::OnCollide(GameObjectEntity* go)
{

}

StaticEntity::STATIC_TYPE StaticEntity::GetStaticType() const
{
	return static_type;
}
