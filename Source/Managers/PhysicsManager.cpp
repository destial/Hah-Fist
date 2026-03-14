#include "PhysicsManager.hpp"
#include "../UI/Debug.hpp"
#include "../Utils/AEOverload.hpp"

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

		for (GameObjectEntity* dynamic2 : qtGameObjects->head->GetPotentialCollisionTargets(dynamic1, ignoredObjects, GameObjectEntity::PhysicsType::DYNAMIC)) {
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

				float tmp{ dynamic2->velocity.x };
				dynamic2->velocity.x = dynamic1->velocity.x;
				dynamic1->velocity.x = tmp;

				dynamic1->OnCollide(dynamic2);
				dynamic2->OnCollide(dynamic1);

				float length = abs(dynamic1->position.y - dynamic2->position.y);
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
				}
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