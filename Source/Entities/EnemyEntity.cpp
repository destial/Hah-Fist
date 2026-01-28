#include "EnemyEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/AEOverload.hpp"
#include "../UI/Debug.hpp"
#include <iostream>

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
	auto corners = Utils::GetCorners(this);
	for (auto& corner : corners) {
		DebugUtils::RenderPoint(corner);
	}
	DebugUtils::RenderLine(corners[0], corners[1]);
	DebugUtils::RenderLine(corners[1], corners[2]);
	DebugUtils::RenderLine(corners[2], corners[3]);
	DebugUtils::RenderLine(corners[3], corners[0]);
}