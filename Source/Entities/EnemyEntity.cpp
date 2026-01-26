#include "EnemyEntity.hpp"
#include "../Utils/MeshRenderer.hpp"

EnemyEntity::EnemyEntity(AEVec2 pos, f32 go_mass) : GameObjectEntity(pos)
{
	mesh = MeshRenderer::GetCenterRectMesh();
	scale = { 5.f,5.f };
	layer = 2;
	pBody = new PhysicsBody(2.f);
}

EnemyEntity::~EnemyEntity()
{
	delete pBody;
}

void EnemyEntity::PreUpdate(const f32& dt)
{
	GameObjectEntity::PreUpdate(dt);
}

void EnemyEntity::Update(const f32& dt)
{
	GameObjectEntity::Update(dt);
	pBody->ApplyGravity(velocity, dt);
}

void EnemyEntity::PostUpdate(const f32& dt)
{
	GameObjectEntity::PostUpdate(dt);
}

void EnemyEntity::Render()
{
	GameObjectEntity::Render();
}