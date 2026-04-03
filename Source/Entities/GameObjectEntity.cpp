/*!
* @file GameObjectEntity.hpp
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 19 January 2026
* @course CSD1451
* @brief Declaration file for a gameobject entity that can collide
*/
#include "GameObjectEntity.hpp"
#include "StaticEntity.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/SceneManager.hpp"
#include "../UI/Debug.hpp"
#include <cmath>

GameObjectEntity::GameObjectEntity()
: BaseEntity{ { 0.f } }, health{ 1.f }, max_health{ 1.f }, damage { 1.f }, is_active{ true },
  shape{ CollisionShape::AABB }, go_type{ PhysicsType::DYNAMIC }, 
  prev_position{ 0.f }, pBody{ new PhysicsBody{0.f} } {}

GameObjectEntity::GameObjectEntity(AEVec2 pos, f32 mass, CollisionShape shape, PhysicsType go_type)
: BaseEntity{ pos }, health{ 1.f }, max_health{ 1.f }, damage{ 1.f }, is_active{ true },
  shape{ shape } , go_type{ go_type },
	prev_position{ 0.f }, pBody{ new PhysicsBody{mass} } {}

GameObjectEntity::~GameObjectEntity() {
	delete pBody;
}
/*!
* @brief Inherited: Pre-update the entity, before any input has been processed
*/
void GameObjectEntity::PreUpdate(const f32& dt) {
	if (!is_active)
		return;

	BaseEntity::PreUpdate(dt);
	this->prev_position = this->position;
	//this->velocity = { 0.f, velocity.y };
}
/*!
* @brief Inherited: Update the entity after input has been processed
*/
void GameObjectEntity::Update(const f32& dt) {
	if (!is_active)
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
/*!
* @brief Inherited: Post-update the entity after everything else has been processed
*/
void GameObjectEntity::PostUpdate(const f32& dt)  {
	if (!is_active)
		return;

	this->position += this->velocity * dt;
	//this->position.y = AEClamp(this->position.y, this->scale.y * 0.5f, Utils::GetWorldHeight() - (this->scale.y * 0.5f));
	if (this->position.y + std::abs(this->scale.y) * 0.5f <= std::abs(this->scale.y) * 0.5f) {
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
/*!
* @brief Inherited: Render the entity to the screen
*/
void GameObjectEntity::Render() {
	if (!is_active)
		return;
	
	BaseEntity::Render();
	if (DebugUtils::IsRendering()) {
		auto corners = Utils::GetCorners(this);

		Color cr = Utils::OBBPoint(this, Utils::GetMouseWorld(true)) ? Color{255, 255, 0, 0} : Color{255, 0, 255, 0};
		DebugUtils::RenderLine(corners[0], corners[1], cr);
		DebugUtils::RenderLine(corners[1], corners[2], cr);
		DebugUtils::RenderLine(corners[2], corners[3], cr);
		DebugUtils::RenderLine(corners[3], corners[0], cr);

		char pos[64];
		sprintf_s(pos, "%d:(%0.2f,%0.2f)", GetId(), this->position.x, this->position.y);
		char typ[64];
		sprintf_s(typ, "type:%d|phys:%d", this->entity_type, this->go_type);
		DebugUtils::RenderText(this->position, pos, true);
		DebugUtils::RenderText(AEVec2{this->position.x, this->position.y - 0.5f}, typ, true);
	}
}
/*!
* @brief Called when this entity collides with another GameObject entity
* @param go - The collided GameObject entity
*/
void GameObjectEntity::OnCollide(GameObjectEntity*) {
	// Empty body
}
