#include "GameObjectEntity.hpp"

GameObjectEntity::GameObjectEntity() : health(1.f),damage(1.f), isActive(true), type(CIRCLE), BaseEntity({0.f})
{
}

GameObjectEntity::GameObjectEntity(AEVec2 pos, SHAPE type) : health(1.f), damage(1.f),isActive(true), type(type),BaseEntity(pos)
{
}

GameObjectEntity::~GameObjectEntity() 
{
}

void GameObjectEntity::PreUpdate(const f32& dt)
{
	BaseEntity::PreUpdate(dt);
	this->velocity = { 0.f, velocity.y };
}

void GameObjectEntity::Update(const f32& dt)
{
	BaseEntity::Update(dt);
}

void GameObjectEntity::PostUpdate(const f32& dt)
{
	BaseEntity::PostUpdate(dt);
}

void GameObjectEntity::Render()
{
	if(isActive)
		BaseEntity::Render();
}
