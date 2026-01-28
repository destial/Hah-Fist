#include "EnemyEntity.hpp"
#include "../Utils/MeshRenderer.hpp"

EnemyEntity::EnemyEntity(AEVec2 pos) : GameObjectEntity(pos)
{
	mesh = MeshRenderer::GetCenterRectMesh();
	scale = { 5.f,5.f };
	layer = 2;
}

EnemyEntity::~EnemyEntity()
{
}

void EnemyEntity::PreUpdate(const f32& dt)
{
	GameObjectEntity::PreUpdate(dt);
}

void EnemyEntity::Update(const f32& dt)
{
	GameObjectEntity::Update(dt);
}

void EnemyEntity::PostUpdate(const f32& dt)
{
	GameObjectEntity::PostUpdate(dt);
}

void EnemyEntity::Render()
{
	GameObjectEntity::Render();
}