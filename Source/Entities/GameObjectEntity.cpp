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
		if (velocity.x > 0)
		{
			velocity.x = AEClamp(velocity.x - velocity.x * (frictionMultiplier * 20.0f) * dt, 0, velocity.x);

		}
		else if (velocity.x < 0)
		{
			velocity.x = AEClamp(velocity.x - velocity.x * (frictionMultiplier * 20.0f) * dt, velocity.x, 0);

		}
		//velocity.x -= velocity.x * (frictionMultiplier * 20.0f) * dt; // later change 5.0 to friction perhaps
		
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
	
}
