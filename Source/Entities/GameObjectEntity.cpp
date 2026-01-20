#include "GameObjectEntity.hpp"

GameObjectEntity::GameObjectEntity() : isActive(true), type(CIRCLE), BaseEntity({0.f})
{
}

GameObjectEntity::GameObjectEntity(AEVec2 pos, SHAPE type) : isActive(true), type(type),BaseEntity(pos)
{
}

GameObjectEntity::~GameObjectEntity() 
{
}

void GameObjectEntity::PreUpdate(const f32& dt)
{
}

void GameObjectEntity::Update(const f32& dt)
{
}

void GameObjectEntity::PostUpdate(const f32& dt)
{
}

void GameObjectEntity::Render()
{
	if(isActive)
		BaseEntity::Render();
}
