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
	BaseEntity::PreUpdate(dt);
	this->prev_position = this->position;
	//this->velocity = { 0.f, velocity.y };
}

void GameObjectEntity::Update(const f32& dt) {
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
	if (isActive) {
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
}

void GameObjectEntity::OnCollide(GameObjectEntity* go) {
	if (go->go_type == PhysicsType::STATIC){//StaticEntity* se = dynamic_cast<StaticEntity*>(go)) {
		StaticEntity* se = dynamic_cast<StaticEntity*>(go);
		if (se == nullptr)
			return;
		AEVec2 static_right_bound{ go->position.x + go->scale.x * 0.5f }, static_left_bound{ go->position.x - go->scale.x * 0.5f };
		


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
	else if (go->go_type == PhysicsType::DYNAMIC) 
	{
		position = prev_position;
	}
}

void CollideDynamicWithStatic(GameObjectEntity* dynamic, GameObjectEntity* _static)
{
	StaticEntity* se = dynamic_cast<StaticEntity*>(_static);
	if (se == nullptr)
		return;

	AEVec2 halfDynamicScale{ dynamic->scale * 0.5f }, halfStaticScale{ _static->scale * 0.5f };
	if (dynamic->position.x <= _static->position.x + _static->scale.x && dynamic->position.x >= _static->position.x - _static->scale.x)
	{
		if (dynamic->prev_position.y - dynamic->scale.y * 0.5f > _static->position.y + _static->scale.y * 0.5f)
		{
			if (dynamic->position.y - dynamic->scale.y * 0.48f > _static->position.y + _static->scale.y * 0.48f)
			{
				f32 next_position = dynamic->position.y + (dynamic->scale.y + _static->scale.y) * 0.5f - (dynamic->position.y - _static->position.y);
				dynamic->position.y = next_position > dynamic->prev_position.y ? next_position : dynamic->prev_position.y;
				dynamic->velocity.y = fabsf(dynamic->velocity.y) > 0.01f ? -dynamic->velocity.y * 0.25f : 0.f;
				dynamic->pBody->is_standing_above = true;
			}
		}
		else
		if (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_PLATFORM)
		{
			return;
		}
		else
		if (dynamic->prev_position.y + dynamic->scale.y * 0.5f < _static->position.y - _static->scale.y * 0.5f)
		{
			if (dynamic->position.y + dynamic->scale.y * 0.48f < _static->position.y - _static->scale.y * 0.48f)
			{
				f32 next_position = dynamic->position.y - (dynamic->scale.y + _static->scale.y) * 0.5f - (_static->position.y - dynamic->position.y);
				dynamic->position.y = next_position > dynamic->prev_position.y ? next_position : dynamic->prev_position.y;
				dynamic->velocity.y = fabsf(dynamic->velocity.y) > 0.01f ? -dynamic->velocity.y * 0.25f : 0.f;
			}
		}
	}
	if (dynamic->position.y <= _static->position.y + _static->scale.y && dynamic->position.y >= _static->position.y - _static->scale.y)
	{
		if (dynamic->prev_position.x - dynamic->scale.x * 0.5f > _static->position.x + _static->scale.x * 0.5f)
		{
			if (dynamic->position.x - dynamic->scale.x * 0.48f > _static->position.x + _static->scale.x * 0.48f)
			{
				f32 next_position = dynamic->position.x + (dynamic->scale.x + _static->scale.x) * 0.5f - (dynamic->position.x - _static->position.x);
				dynamic->position.x = next_position > dynamic->prev_position.x ? next_position : dynamic->prev_position.x;
				dynamic->velocity.x = fabsf(dynamic->velocity.x) > 0.01f ? -dynamic->velocity.x * 0.25f : 0.f;
			}
		}
		else
			if (dynamic->prev_position.x + dynamic->scale.x * 0.5f < _static->position.x - _static->scale.x * 0.5f)
			{
				if (dynamic->position.x + dynamic->scale.x * 0.48f < _static->position.x - _static->scale.x * 0.48f)
				{
					f32 next_position = dynamic->position.x - (dynamic->scale.x + _static->scale.x) * 0.5f - (_static->position.x - dynamic->position.x);
					dynamic->position.x = next_position > dynamic->prev_position.x ? next_position : dynamic->prev_position.x;
					dynamic->velocity.x = fabsf(dynamic->velocity.x) > 0.01f ? -dynamic->velocity.x * 0.25f : 0.f;
				}
			}
	}

}