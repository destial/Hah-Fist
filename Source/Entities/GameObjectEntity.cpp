#include "GameObjectEntity.hpp"
#include "StaticEntity.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/SceneManager.hpp"
#include "../UI/Debug.hpp"
#include <cmath>

GameObjectEntity::GameObjectEntity()
: BaseEntity{ { 0.f } }, health{ 1.f }, max_health{ 1.f }, damage { 1.f }, isActive{ true },
  shape{ CollisionShape::AABB }, go_type{ PhysicsType::DYNAMIC }, 
  prev_position{ 0.f }, pBody{ new PhysicsBody{0.f} } {}

GameObjectEntity::GameObjectEntity(AEVec2 pos, f32 mass, CollisionShape shape, PhysicsType go_type)
: BaseEntity{ pos }, health{ 1.f }, max_health{ 1.f }, damage{ 1.f }, isActive{ true },
  shape{ shape } , go_type{ go_type },
	prev_position{ 0.f }, pBody{ new PhysicsBody{mass} } {}

GameObjectEntity::~GameObjectEntity() {
	delete pBody;
}

void GameObjectEntity::PreUpdate(const f32& dt) {
	if (!isActive)
		return;

	BaseEntity::PreUpdate(dt);
	this->prev_position = this->position;
	//this->velocity = { 0.f, velocity.y };
}

void GameObjectEntity::Update(const f32& dt) {
	if (!isActive)
		return;

	BaseEntity::Update(dt);

	invulnerabilityDuration -= dt;
	timeElapsedSinceLastDamage += dt;

	if (go_type == PhysicsType::DYNAMIC) {
		pBody->UpdateStates(this->velocity, this->position, this->scale);
		pBody->ApplyGravity(this->velocity, dt);

		/*pBody->air_strength = AEClamp(pBody->air_strength - 2 * dt, 0.0, 1.0);*/
	}
}

void GameObjectEntity::PostUpdate(const f32& dt)  {
	if (!isActive)
		return;

	this->position += this->velocity * dt;
	//this->position.y = AEClamp(this->position.y, this->scale.y * 0.5f, Utils::GetWorldHeight() - (this->scale.y * 0.5f));
	if (this->position.y + this->scale.y * 0.5f <= this->scale.y * 0.5f) {
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
		return;
	}
	
	if (pBody->is_standing_above) {
		velocity.x -= velocity.x * (frictionMultiplier * 20.0f) * dt; // later change 5.0 to friction perhaps
		if (std::abs(velocity.x) < 0.3) {
			velocity.x = 0.0;
		}
	}
	BaseEntity::PostUpdate(dt);
}

void GameObjectEntity::Render() {
	if (!isActive)
		return;
	
	BaseEntity::Render();
	auto corners = Utils::GetCorners(this);
	DebugUtils::RenderLine(corners[0], corners[1]);
	DebugUtils::RenderLine(corners[1], corners[2]);
	DebugUtils::RenderLine(corners[2], corners[3]);
	DebugUtils::RenderLine(corners[3], corners[0]);

	if (DebugUtils::IsRendering()) {
		char pos[64];
		sprintf_s(pos, "(%0.2f,%0.2f)", this->position.x, this->position.y);
		DebugUtils::RenderText(this->position, pos, true);
	}
}

void GameObjectEntity::OnCollide(GameObjectEntity* go) {
	if (go->go_type == PhysicsType::STATIC || go->go_type == PhysicsType::MOVING_STATIC){//StaticEntity* se = dynamic_cast<StaticEntity*>(go)) {
		StaticEntity* se = dynamic_cast<StaticEntity*>(go);
		if (se == nullptr)
			return;
		if (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_PLATFORM) { // Collision with a platform
			if (position.x <= go->position.x + go->scale.x && position.x >= go->position.x - go->scale.x)
			{
				if (prev_position.y - scale.y * 0.5f > go->position.y + go->scale.y * 0.5f)
				{
					if (position.y - scale.y * 0.48f > go->position.y + go->scale.y * 0.48f)
					{
						f32 next_position = (scale.y + go->scale.y) * 0.5f - (position.y - go->position.y) + position.y;
						position.y = next_position > prev_position.y ? next_position : prev_position.y;
						velocity.y = fabsf(velocity.y) > 0.01f ? -velocity.y * 0.25f : 0.f;
						pBody->is_standing_above = true;
					}
				}
			}
		}
		else if(se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_WALL) { // Collision with a wall
			if (position.x <= go->position.x + go->scale.x && position.x >= go->position.x - go->scale.x)
			{
				if (prev_position.y - scale.y * 0.5f > go->position.y + go->scale.y * 0.5f)
				{
					if (position.y - scale.y * 0.48f > go->position.y + go->scale.y * 0.48f)
					{
						f32 next_position = position.y + (scale.y + go->scale.y) * 0.5f - (position.y - go->position.y);
						position.y = next_position > prev_position.y ? next_position : prev_position.y;
						velocity.y = fabsf(velocity.y) > 0.01f ? -velocity.y * 0.25f : 0.f;
						pBody->is_standing_above = true;
					}
				}
				else
				if (prev_position.y + scale.y * 0.5f < go->position.y - go->scale.y * 0.5f)
				{
					if (position.y + scale.y * 0.48f < go->position.y - go->scale.y * 0.48f)
					{
						f32 next_position = position.y - (scale.y + go->scale.y) * 0.5f - (go->position.y - position.y);
						position.y = next_position > prev_position.y ? next_position : prev_position.y;
						velocity.y = fabsf(velocity.y) > 0.01f ? -velocity.y * 0.25f : 0.f;
					}
				}
			}
			if (position.y <= go->position.y + go->scale.y && position.y >= go->position.y - go->scale.y)
			{
				if (prev_position.x - scale.x * 0.5f > go->position.x + go->scale.x * 0.5f)
				{
					if (position.x - scale.x * 0.48f > go->position.x + go->scale.x * 0.48f)
					{
						f32 next_position = position.x + (scale.x + go->scale.x) * 0.5f - (position.x - go->position.x);
						position.x = next_position > prev_position.x ? next_position : prev_position.x;
						velocity.x = fabsf(velocity.x) > 0.01f ? -velocity.x * 0.25f : 0.f;
					}
				}
				else
				if (prev_position.x + scale.x * 0.5f < go->position.x - go->scale.x * 0.5f)
				{
					if (position.x + scale.x * 0.48f < go->position.x - go->scale.x * 0.48f)
					{
						f32 next_position = position.x - (scale.x + go->scale.x) * 0.5f - (go->position.x - position.x);
						position.x = next_position > prev_position.x ? next_position : prev_position.x;
						velocity.x = fabsf(velocity.x) > 0.01f ? -velocity.x * 0.25f : 0.f;
					}
				}
			}
		}
	}
	else if (go->go_type == PhysicsType::DYNAMIC) {
		position = prev_position;
		
		//AEVec2 thisToGO = go->position - position;
		//if (AEVec2DotProduct(&velocity, &thisToGO) > 1) {
		//	//Velocity Trading
		//	
		//	AEVec2 tmp{ velocity };
		//	AEVec2 tmp2{ go->velocity };
		//	f32 mass_total = pBody->mass + go->pBody->mass;


		//	if (position.y >= go->position.y + go->scale.y * 0.5f + scale.y * 0.49f) {
		//		position = prev_position;
		//		velocity.y = 0.0f;
		//		go->velocity.y = (go->velocity.y * (go->pBody->mass - pBody->mass) + tmp.y * 2 * pBody->mass) / mass_total;
		//	}
		//	else {
		//		go->position = go->prev_position;
		//		go->velocity.y = 0.0f;
		//		velocity.y = (velocity.y * (pBody->mass - go->pBody->mass) + tmp2.y * 2 * go->pBody->mass) / mass_total;
		//		velocity.x = (velocity.x * (pBody->mass - go->pBody->mass) + tmp2.x * 2 * go->pBody->mass) / mass_total;
		//		go->velocity.x = (go->velocity.x * (go->pBody->mass - pBody->mass) + tmp.x * 2 * pBody->mass) / mass_total;

		//	}
		//}
	}
}
