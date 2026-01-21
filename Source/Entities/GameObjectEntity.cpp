#include "GameObjectEntity.hpp"
#include "../Utils/AEOverload.hpp"

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
	this->position += this->velocity * dt;
	this->position.y = AEClamp(this->position.y, this->scale.y * 0.5f, Utils::GetWorldHeight() - (this->scale.y * 0.5f));
	/*if (this->position.x <= 0.f) {
		velocity.x = 0;
	}*/
	if (this->position.y <= this->scale.y * 0.5f) {
		velocity.y = 0;
	}
	BaseEntity::PostUpdate(dt);
}

void GameObjectEntity::Render()
{
	if(isActive)
		BaseEntity::Render();
}
