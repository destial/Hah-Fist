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

void PhysicsManager::Update(const f32& dt)
{
	//Reset collision state to false;
	for (auto& go : gameObjects) { go->pBody->is_standing_above = false; }
	float tCollide{};
	// Collision: DYNAMIC vs TRIGGER
	for (GameObjectEntity* trigger : gameObjects) {
		//If trigger inactive, continue
		if (!trigger->isActive) { continue; }

		//If GameObject is not a trigger, continue
		if (trigger->go_type != GameObjectEntity::PhysicsType::TRIGGER) { continue; }
		
		for (GameObjectEntity* dynamic : qtGameObjects->head->GetPotentialCollisionTargets(trigger, GameObjectEntity::PhysicsType::DYNAMIC)) {
			//Checks if either go is inactive, if so, skip this check
			if (!dynamic->isActive) { continue; }
				
			if (Utils::OBB(trigger, dynamic)) {
				trigger->OnCollide(dynamic);
			}
		}

		for (GameObjectEntity* _static : qtGameObjects->head->GetPotentialCollisionTargets(trigger, GameObjectEntity::PhysicsType::STATIC)) {
			//Checks if either go is inactive, if so, skip this check
			if (!_static->isActive) { continue; }

			if (Utils::OBB(trigger, _static)) {
				trigger->OnCollide(_static);
			}
		}
	}

	// Collision: DYNAMIC vs DYNAMIC
	std::vector<GameObjectEntity*> ignoredObjects{};
	for (GameObjectEntity* dynamic1 : gameObjects) {
		ignoredObjects.push_back(dynamic1);
		//If trigger inactive, continue
		if (!dynamic1->isActive) { continue; }

		//If GameObject is not a trigger, continue
		if (dynamic1->go_type != GameObjectEntity::PhysicsType::DYNAMIC) { continue; }

		for (GameObjectEntity* dynamic2 : qtGameObjects->head->GetPotentialCollisionTargets(dynamic1, ignoredObjects, GameObjectEntity::PhysicsType::DYNAMIC)) {
			//Checks if either go is inactive, if so, skip this check
			if (!dynamic1->isActive) { continue; }
			
			if (Utils::OBB(dynamic1, dynamic2)) {
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
			else if (Utils::DynamicAABB(dynamic1, dynamic2, tCollide, dt)) {
				dynamic1->position.x = dynamic1->velocity.x * tCollide + dynamic1->prev_position.x;
				dynamic1->position.y = dynamic1->velocity.y * tCollide + dynamic1->prev_position.y;
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

	// Collision: DYNAMIC vs STATIC
	for (GameObjectEntity* _static : gameObjects) {
		//If trigger inactive, continue
		if (!_static->isActive) { continue; }

		//If GameObject is not a trigger, continue
		if (_static->go_type != GameObjectEntity::PhysicsType::STATIC) { continue; }

		for (GameObjectEntity* dynamic : qtGameObjects->head->GetPotentialCollisionTargets(_static, GameObjectEntity::PhysicsType::DYNAMIC)) {
			//Checks if either go is inactive, if so, skip this check
			if (!dynamic->isActive) { continue; }
			if (Utils::OBB(_static, dynamic)) {
				_static->OnCollide(dynamic);
				dynamic->OnCollide(_static);
				float length = abs(dynamic->position.y - _static->position.y);
				if (length <= dynamic->scale.y * 0.5f + _static->scale.y * 0.5f)
				{
					if (dynamic->position.y > _static->position.y)
					{
						dynamic->pBody->is_standing_above = true;
					}
				}
			}
			else if (Utils::DynamicAABB(dynamic, _static, tCollide, dt)) {
				dynamic->position.x = dynamic->velocity.x * tCollide + dynamic->prev_position.x;
				dynamic->position.y = dynamic->velocity.y * tCollide + dynamic->prev_position.y;
				_static->OnCollide(dynamic);
				dynamic->OnCollide(_static);
				float length = abs(dynamic->position.y - _static->position.y);
				if (length <= dynamic->scale.y * 0.5f + _static->scale.y * 0.5f)
				{
					if (dynamic->position.y > _static->position.y)
					{
						dynamic->pBody->is_standing_above = true;
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