#include "GameObjectEntity.hpp"
#include "../Utils/AEOverload.hpp"
#include "../UI/Debug.hpp"

GameObjectEntity::GameObjectEntity() : health(1.f), damage(1.f), isActive(true), type(SHAPE::AABB), go_type(KINEMATIC::DYNAMIC), prev_position({ 0.f }), BaseEntity({ 0.f })
{
	pBody = new PhysicsBody{};
}

GameObjectEntity::GameObjectEntity(AEVec2 pos, f32 mass, SHAPE type) : health(1.f), damage(1.f),isActive(true), type(type), go_type(DYNAMIC), prev_position(pos), BaseEntity(pos)
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
	this->prev_position = this->position;
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
	if (isActive) {
		BaseEntity::Render();
		auto corners = Utils::GetCorners(this);
		DebugUtils::RenderLine(corners[0], corners[1], { 255, 255, 0, 0 });
		DebugUtils::RenderLine(corners[1], corners[2], { 255, 0, 255, 0 });
		DebugUtils::RenderLine(corners[2], corners[3], { 255, 0, 0, 255 });
		DebugUtils::RenderLine(corners[3], corners[0], { 255, 255, 255, 0 });
	}
}

void GameObjectEntity::OnCollide(GameObjectEntity* go)
{
	if (go->go_type == GameObjectEntity::KINEMATIC::STATIC) {
		AEVec2 down = { 0, -1.f };
		if (velocity * down > 1 && position.y >= go->position.y + go->scale.y * 0.5f + scale.y * 0.49f)
		{
			position = prev_position;
			velocity.y = 0.0f;
		}
	}
	else if (go->go_type == GameObjectEntity::KINEMATIC::DYNAMIC) {
		AEVec2 thisToGO = go->position - position;
		if (AEVec2DotProduct(&velocity, &thisToGO) > 1)
		{
			//Velocity Trading

			AEVec2 tmp{ velocity };
			AEVec2 tmp2{ go->velocity };
			f32 mass_total = pBody->mass + go->pBody->mass;


			if (position.y >= go->position.y + go->scale.y * 0.5f + scale.y * 0.49f)
			{
				position = prev_position;
				velocity.y = 0.0f;
				go->velocity.y = (go->velocity.y * (go->pBody->mass - pBody->mass) + tmp.y * 2 * pBody->mass) / mass_total;
			}
			else
			{
				go->position = go->prev_position;
				go->velocity.y = 0.0f;
				velocity.y = (velocity.y * (pBody->mass - go->pBody->mass) + tmp2.y * 2 * go->pBody->mass) / mass_total;
				velocity.x = (velocity.x * (pBody->mass - go->pBody->mass) + tmp2.x * 2 * go->pBody->mass) / mass_total;
				go->velocity.x = (go->velocity.x * (go->pBody->mass - pBody->mass) + tmp.x * 2 * pBody->mass) / mass_total;

			}
		}
	}
}
