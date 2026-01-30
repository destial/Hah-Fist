#include "EnemyEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Managers/AssetManager.hpp"


EnemyEntity::EnemyEntity(AEVec2 pos) : GameObjectEntity(pos)
{
	sprite = AssetManager::GetSpriteSheet("Assets/test_enemy.png", 3, 3);
	mesh = nullptr;
	animationTimer = 0.f;
	animationFrame = 1.f / (3.f * 3.f);
	currentRow = currentCol = 0;
	scale = { 5.f * (static_cast<f32>(sprite->image->width) / sprite->image->height), 5.f };
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
	currentRow = 0;
	if (velocity.x > 0) {
		currentRow = 1;
	}
	if (velocity.x < 0) {
		currentRow = 2;
	}
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 3) {
			currentCol = 0;
		}
	}
}

void EnemyEntity::Render()
{
	sprite->Render(transform, color, currentRow, currentCol);
	GameObjectEntity::Render();
}