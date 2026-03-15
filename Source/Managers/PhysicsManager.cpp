#include "PhysicsManager.hpp"
#include "../UI/Debug.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Entities/StaticEntity.hpp"

PhysicsManager::PhysicsManager(Physics::AABB _worldBounds, size_t _maxEntriesPerNode)
	: worldBounds(_worldBounds), maxEntriesPerNode(_maxEntriesPerNode), gameObjects(0)
{

}

PhysicsManager::~PhysicsManager()
{
	gameObjects.clear();
	if (qtGameObjects != nullptr)
	{
		delete qtGameObjects;
	}
}

void PhysicsManager::PreUpdate(const f32& dt)
{
	if (qtGameObjects != nullptr)
	{
		delete qtGameObjects;
	}
	qtGameObjects = new QuadTree::Tree(worldBounds, gameObjects, maxEntriesPerNode);
}

void PhysicsManager::PostUpdate(const f32& dt)
{
	//Reset collision state to false;
	for (auto& go : gameObjects) { go->pBody->is_standing_above = false; }
	float tCollide{};
	std::vector<GameObjectEntity*> ignoredObjects{};

	// Collision: DYNAMIC vs TRIGGER
	for (GameObjectEntity* trigger : gameObjects) {
		//If trigger inactive, continue
		if (!trigger->isActive) { continue; }

		//If GameObject is not a trigger, continue
		if (trigger->go_type != GameObjectEntity::PhysicsType::TRIGGER) { continue; }
		
		ignoredObjects.push_back(trigger);

		for (GameObjectEntity* trigger2 : qtGameObjects->head->GetPotentialCollisionTargets(trigger, ignoredObjects, GameObjectEntity::PhysicsType::TRIGGER)) {
			if (Utils::OBB(trigger, trigger2)) {
				trigger->OnCollide(trigger2);
				trigger2->OnCollide(trigger);
			}
		}

		for (GameObjectEntity* dynamic : qtGameObjects->head->GetPotentialCollisionTargets(trigger, GameObjectEntity::PhysicsType::DYNAMIC)) {	
			if (Utils::OBB(trigger, dynamic)) {
				trigger->OnCollide(dynamic);
			}
		}

		for (GameObjectEntity* _static : qtGameObjects->head->GetPotentialCollisionTargets(trigger, GameObjectEntity::PhysicsType::STATIC)) {
			if (Utils::OBB(trigger, _static)) {
				trigger->OnCollide(_static);
			}
		}
	}

	
	// Collision: DYNAMIC vs MOVING_STATIC
	for (GameObjectEntity* _static : gameObjects) {
		//If trigger inactive, continue
		if (!_static->isActive) { continue; }

		//If GameObject is not a trigger, continue
		if (_static->go_type != GameObjectEntity::PhysicsType::MOVING_STATIC) { continue; }

		for (GameObjectEntity* dynamic : qtGameObjects->head->GetPotentialCollisionTargets(_static, GameObjectEntity::PhysicsType::DYNAMIC)) {
			bool has_collision{ false };
			if (!Utils::OBB(_static, dynamic))
			{
				if (Utils::DynamicAABB(dynamic, _static, tCollide, dt))
				{
					dynamic->prev_position.x += dynamic->velocity.x * tCollide;
					dynamic->prev_position.y += dynamic->velocity.y * tCollide;
					has_collision = true;
				}
			}
			else
			{
				has_collision = true;
			}

			if (has_collision)
			{
				_static->OnCollide(dynamic);
				dynamic->OnCollide(_static);
				HandleStaticDynamicCollisionResponse(_static, dynamic);
			}
		}
	}

	// Collision: DYNAMIC vs STATIC
	for (GameObjectEntity* _static : gameObjects) {
		//If trigger inactive, continue
		if (!_static->isActive) { continue; }

		//If GameObject is not a trigger, continue
		if (_static->go_type != GameObjectEntity::PhysicsType::STATIC) { continue; }

		for (GameObjectEntity* dynamic : qtGameObjects->head->GetPotentialCollisionTargets(_static, GameObjectEntity::PhysicsType::DYNAMIC)) {
			bool has_collision{ false };
			if (!Utils::OBB(_static, dynamic))
			{
				if (Utils::DynamicAABB(dynamic, _static, tCollide, dt))
				{
					dynamic->prev_position.x += dynamic->velocity.x * tCollide;
					dynamic->prev_position.y += dynamic->velocity.y * tCollide;
					has_collision = true;
				}
			}
			else
			{
				has_collision = true;
			}
			
			if (has_collision)
			{
				_static->OnCollide(dynamic);
				dynamic->OnCollide(_static);
				HandleStaticDynamicCollisionResponse(_static, dynamic);
			}
		}
	}

	// Collision: DYNAMIC vs DYNAMIC
	ignoredObjects.clear();
	for (GameObjectEntity* dynamic1 : gameObjects) {
		ignoredObjects.push_back(dynamic1);
		//If trigger inactive, continue
		if (!dynamic1->isActive) { continue; }

		//If GameObject is not a trigger, continue
		if (dynamic1->go_type != GameObjectEntity::PhysicsType::DYNAMIC) { continue; }

		if (dynamic1->invulnerabilityDuration > 0.f) { continue; }

		for (GameObjectEntity* dynamic2 : qtGameObjects->head->GetPotentialCollisionTargets(dynamic1, ignoredObjects, GameObjectEntity::PhysicsType::DYNAMIC)) {
			if (dynamic2->invulnerabilityDuration > 0.f) { continue; }
			if (dynamic1->entity_type == GameObjectEntity::EntityType::ENEMY && dynamic2->entity_type == GameObjectEntity::EntityType::ENEMY) { continue; }
			bool has_collision{ false };
			if (!Utils::OBB(dynamic1, dynamic2)) {
				if (Utils::DynamicAABB(dynamic1, dynamic2, tCollide, dt))
				{
					dynamic1->prev_position += dynamic1->velocity * tCollide;
					dynamic2->prev_position += dynamic2->velocity * tCollide;
					has_collision = true;
				}
			}
			else
			{
				has_collision = true;
			}

			if (has_collision)
			{

				//float tmp{ dynamic2->velocity.x };
				//dynamic2->velocity.x = dynamic1->velocity.x;
				//dynamic1->velocity.x = tmp;
				HandleDynamicDynamicCollisionResponse(dynamic1, dynamic2);
				dynamic1->OnCollide(dynamic2);
				dynamic2->OnCollide(dynamic1);

				/*float length = abs(dynamic1->position.y - dynamic2->position.y);
				if (length <= dynamic1->scale.y * 0.5f + dynamic2->scale.y * 0.5f)
				{
					if (dynamic1->position.y > dynamic2->position.y)
					{
						dynamic1->pBody->is_standing_above = true;
					}
					else
					{
						dynamic2->pBody->is_standing_above = true;
					}
				}*/
			}
		}
	}
}

void PhysicsManager::Render()
{
	if (qtGameObjects)
		qtGameObjects->RenderDebug();
}

void PhysicsManager::Clear() {
	gameObjects.clear();
	if (qtGameObjects != nullptr)
	{
		delete qtGameObjects;
	}
	qtGameObjects = nullptr;
}

void PhysicsManager::PushGameObject(GameObjectEntity* gameObject)
{
	gameObjects.push_back(gameObject);
}

void PhysicsManager::HandleStaticDynamicCollisionResponse(GameObjectEntity* _static, GameObjectEntity* _dynamic)
{
	StaticEntity* se = dynamic_cast<StaticEntity*>(_static);
	if (se == nullptr)
		return;
	
	bool is_standing_on_this_object = false;
	if (_dynamic->prev_position.y - _dynamic->GetHalfSize().y >= _static->position.y + _static->GetHalfSize().y)
	{
		//Snap position back to previous position
		_dynamic->position.y = _dynamic->prev_position.y;

		//If velocity is trying to move into static, instead flip it with dampening
		_dynamic->velocity.y = _dynamic->velocity.y < 0.f ? _dynamic->velocity.y * -0.25f : _dynamic->velocity.y;

		//Set the dynamic to standing above something
		_dynamic->pBody->is_standing_above = true;

		is_standing_on_this_object = true;
	}
	//Exit collision for platforms as they only care for collisions from above.
	if (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_PLATFORM) { return; }
	if (_dynamic->prev_position.y + _dynamic->GetHalfSize().y <= _static->position.y - _static->GetHalfSize().y)
	{
		//Snap position back to previous position
		_dynamic->position.y = _dynamic->prev_position.y;

		//If velocity is trying to move into static, instead flip it with dampening
		_dynamic->velocity.y = _dynamic->velocity.y > 0.f ? _dynamic->velocity.y * -0.25f : _dynamic->velocity.y;
	}
	if (!is_standing_on_this_object)
	{
		if (_dynamic->prev_position.x - _dynamic->GetHalfSize().x >= _static->position.x + _static->GetHalfSize().x)
		{
			//Snap position back to previous position
			_dynamic->position.x = _dynamic->prev_position.x;

			//If velocity is trying to move into static, instead flip it with dampening
			_dynamic->velocity.x = _dynamic->velocity.x < 0.f ? _dynamic->velocity.x * -0.25f : _dynamic->velocity.x;
		}
		else if (_dynamic->prev_position.x + _dynamic->GetHalfSize().x <= _static->position.x - _static->GetHalfSize().x)
		{
			//Snap position back to previous position
			_dynamic->position.x = _dynamic->prev_position.x;

			//If velocity is trying to move into static, instead flip it with dampening
			_dynamic->velocity.x = _dynamic->velocity.x > 0.f ? _dynamic->velocity.x * -0.25f : _dynamic->velocity.x;
		}
	}
}

void PhysicsManager::HandleDynamicDynamicCollisionResponse(GameObjectEntity* first, GameObjectEntity* second)
{
	if (first->prev_position.y - first->GetHalfSize().y >= second->prev_position.y + second->GetHalfSize().y)
	{
		//Snap both positions back to previous position
		first->position.y = first->prev_position.y;
		second->position.y = second->prev_position.y;

		//Trade velocities with each other
		f32 tmp{ first->velocity.y };
		first->velocity.y = second->velocity.y;
		second->velocity.y = tmp;

		//Set the dynamic to standing above something
		first->pBody->is_standing_above = true;
	}
	else if (first->prev_position.y + first->GetHalfSize().y <= second->prev_position.y - second->GetHalfSize().y)
	{
		//Snap both positions back to previous position
		first->position.y = first->prev_position.y;
		second->position.y = second->prev_position.y;

		//Trade velocities with each other
		f32 tmp{ first->velocity.y };
		first->velocity.y = second->velocity.y;
		second->velocity.y = tmp;

		//Set the dynamic to standing above something
		second->pBody->is_standing_above = true;
	}

	if (first->prev_position.x - first->GetHalfSize().x >= second->prev_position.x + second->GetHalfSize().x)
	{
		//Snap both positions back to previous position
		first->position.x = first->prev_position.x;
		second->position.x = second->prev_position.x;

		//Trade velocities with each other
		f32 tmp{ first->velocity.x };
		first->velocity.x = second->velocity.x;
		second->velocity.x = tmp;

		//Add a little extra bounce
		first->velocity.x -= 1.0f;
		second->velocity.x += 1.0f;
	}
	else if (first->prev_position.x + first->GetHalfSize().x <= second->prev_position.x - second->GetHalfSize().x)
	{
		//Snap both positions back to previous position
		first->position.x = first->prev_position.x;
		second->position.x = second->prev_position.x;

		//Trade velocities with each other
		f32 tmp{ first->velocity.x };
		first->velocity.x = second->velocity.x;
		second->velocity.x = tmp;

		//Add a little extra bounce
		first->velocity.x += 1.0f;
		second->velocity.x -= 1.0f;
	}
}