#include "GameObjectEntity.hpp"
#include "../Utils/AEOverload.hpp"

GameObjectEntity::GameObjectEntity() : health(1.f),damage(1.f), isActive(true), type(CIRCLE), go_type(DYNAMIC), BaseEntity({0.f})
{
	pBody = new PhysicsBody{};
}

GameObjectEntity::GameObjectEntity(AEVec2 pos, f32 mass, SHAPE type) : health(1.f), damage(1.f),isActive(true), type(type), go_type(DYNAMIC), BaseEntity(pos)
{
	pBody = new PhysicsBody{ mass };
}

GameObjectEntity::~GameObjectEntity() 
{
	delete pBody;
}

void GameObjectEntity::PreUpdate(const f32& dt)
{
	BaseEntity::PreUpdate(dt);
	//this->velocity = { 0.f, velocity.y };
}

void GameObjectEntity::Update(const f32& dt)
{
	BaseEntity::Update(dt);
	if (go_type == DYNAMIC) {
		pBody->UpdateStates(this->velocity, this->position, this->scale);
		pBody->ApplyGravity(this->velocity, dt);
	}
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
	
	if (velocity.y == 0)
	{
		velocity.x -= velocity.x * 20.0f * dt; // later change 5.0 to friction perhaps
		if (abs(velocity.x) < 0.3)
		{
			velocity.x = 0.0;
		}
	}
	BaseEntity::PostUpdate(dt);
}

void GameObjectEntity::Render()
{
	if(isActive)
		BaseEntity::Render();
}
