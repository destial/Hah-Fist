#include "PhysicsManager.hpp"
#include "../UI/Debug.hpp"
#include "../Utils/AEOverload.hpp"

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
	for (auto& go : gameObjects) { go->pBody->is_colliding = false; }

	// Collision detection 
	std::vector<GameObjectEntity*> ignoredObjects{};
	for (GameObjectEntity* go : gameObjects) {
		ignoredObjects.push_back(go);
		for (GameObjectEntity* go2 : qtGameObjects->head->GetPotentialCollisionTargets(go, ignoredObjects)) {
			//Checks if either go is inactive, if so, skip this check
			if (!go->isActive || !go2->isActive)
				continue;

			DebugUtils::RenderPoint(go->position + go->scale * 0.25f, { 255, 255, 255, 0 });
			DebugUtils::RenderPoint(go2->position + go2->scale * -0.25f, { 255, 0, 255, 255 });
			if (Utils::OBB(go, go2)) {
				//go->color = { 255, 0, 0, 0 };
				//AEVec2 go1to2 = go2->position - go->position;
				go->pBody->is_colliding = true;
				go2->pBody->is_colliding = true;

				go->OnCollide(go2);
				go2->OnCollide(go);
				//if (AEVec2DotProduct(&go->velocity, &go1to2) > 1)
				//{
				//	//Velocity Trading

				//	AEVec2 tmp{ go->velocity };
				//	AEVec2 tmp2{ go2->velocity };
				//	f32 mass_total = go->pBody->mass + go2->pBody->mass;

				//	if (go->go_type == GameObjectEntity::KINEMATIC::DYNAMIC && go2->go_type == GameObjectEntity::KINEMATIC::DYNAMIC) {
				//		if (go->position.y > go2->position.y + go2->scale.y * 0.5f)
				//		{
				//			go->position = go->prev_position;
				//			go->velocity.y = 0.0f;
				//			go2->velocity.y = (go2->velocity.y * (go2->pBody->mass - go->pBody->mass) + tmp.y * 2 * go->pBody->mass) / mass_total;
				//		}
				//		else
				//		{
				//			go2->position = go2->prev_position;
				//			go2->velocity.y = 0.0f;
				//			go->velocity.y = (go->velocity.y * (go->pBody->mass - go2->pBody->mass) + tmp2.y * 2 * go2->pBody->mass) / mass_total;
				//			go->velocity.x = (go->velocity.x * (go->pBody->mass - go2->pBody->mass) + tmp2.x * 2 * go2->pBody->mass) / mass_total;
				//			go2->velocity.x = (go2->velocity.x * (go2->pBody->mass - go->pBody->mass) + tmp.x * 2 * go->pBody->mass) / mass_total;

				//		}
				//	}
				//}

				/*AEVec2 down = { 0, -1.f };
				f32 dotdown1 = go->velocity * down;
				f32 dotdown2 = go2->velocity * down;
				if (go->go_type == GameObjectEntity::KINEMATIC::DYNAMIC && go2->go_type == GameObjectEntity::KINEMATIC::STATIC) {
					if (dotdown1 > 1 && go->position.y >= go2->position.y + go2->scale.y * 0.5f + go->scale.y * 0.49f)
					{
						go->position = go->prev_position;
						go->velocity.y = 0.0f;
					}
				}

				if (go2->go_type == GameObjectEntity::KINEMATIC::DYNAMIC && go->go_type == GameObjectEntity::KINEMATIC::STATIC) {
					if (dotdown2 > 1 && go2->position.y >= go->position.y + go->scale.y * 0.5f + go2->scale.y * 0.49f)
					{
						go2->position = go2->prev_position;
						go2->velocity.y = 0.0f;
					}
				}*/
			}
		}
	}
	//for (int i{}; i < gameObjects.size(); i++) {
	//	// Starts loop only from the next object
	//	for (int j{ i + 1 }; j < gameObjects.size(); j++) {
	//		GameObjectEntity* go = gameObjects[i];
	//		GameObjectEntity* go2 = gameObjects[j];
	//		//Checks if either go is inactive, if so, skip this check
	//		if (!go->isActive || !go2->isActive)
	//			continue;

	//		DebugUtils::RenderPoint(go->position + go->scale * 0.25f, { 255, 255, 255, 0 });
	//		DebugUtils::RenderPoint(go2->position + go2->scale * -0.25f, { 255, 0, 255, 255 });
	//		if (Utils::OBB(go, go2)) {
	//			//go->color = { 255, 0, 0, 0 };
	//			AEVec2 go1to2 = go2->position - go->position;
	//			go->pBody->is_colliding = true;
	//			go2->pBody->is_colliding = true;

	//			if (AEVec2DotProduct(&go->velocity, &go1to2) > 1)
	//			{
	//				//Velocity Trading

	//				AEVec2 tmp{ go->velocity };
	//				AEVec2 tmp2{ go2->velocity };
	//				f32 mass_total = go->pBody->mass + go2->pBody->mass;

	//				if (go->go_type == GameObjectEntity::KINEMATIC::DYNAMIC && go2->go_type == GameObjectEntity::KINEMATIC::DYNAMIC) {
	//					if (go->position.y > go2->position.y + go2->scale.y * 0.5f)
	//					{
	//						go->position = go->prev_position;
	//						go->velocity.y = 0.0f;
	//						go2->velocity.y = (go2->velocity.y * (go2->pBody->mass - go->pBody->mass) + tmp.y * 2 * go->pBody->mass) / mass_total;
	//					}
	//					else
	//					{
	//						go2->position = go2->prev_position;
	//						go2->velocity.y = 0.0f;
	//						go->velocity.y = (go->velocity.y * (go->pBody->mass - go2->pBody->mass) + tmp2.y * 2 * go2->pBody->mass) / mass_total;
	//						go->velocity.x = (go->velocity.x * (go->pBody->mass - go2->pBody->mass) + tmp2.x * 2 * go2->pBody->mass) / mass_total;
	//						go2->velocity.x = (go2->velocity.x * (go2->pBody->mass - go->pBody->mass) + tmp.x * 2 * go->pBody->mass) / mass_total;

	//					}
	//				}
	//			}

	//			AEVec2 down = { 0, -1.f };
	//			f32 dotdown1 = AEVec2DotProduct(&go->velocity, &down);
	//			f32 dotdown2 = AEVec2DotProduct(&go2->velocity, &down);

	//			if (go->go_type == GameObjectEntity::KINEMATIC::DYNAMIC && go2->go_type == GameObjectEntity::KINEMATIC::STATIC) {
	//				if (dotdown1 > 1)
	//				{
	//					go->position = go->prev_position;
	//					go->velocity.y = 0.0f;
	//				}
	//			}

	//			if (go2->go_type == GameObjectEntity::KINEMATIC::DYNAMIC && go->go_type == GameObjectEntity::KINEMATIC::STATIC) {
	//				if (dotdown2 > 1)
	//				{
	//					go2->position = go2->prev_position;
	//					go2->velocity.y = 0.0f;
	//				}
	//			}
	//		}


	//		/*AEVec2 go1_push = go->position - go2->position;
	//		AEVec2 go2_push = go2->position - go->position;
	//		Utils::SnapVectorToAxis(&go1_push, &go1_push);
	//		Utils::SnapVectorToAxis(&go2_push, &go2_push);
	//		go->velocity += go1_push;
	//		go2->velocity += go2_push;*/
	//		/*if (go1_push.y > go1_push.x || go2_push.y > go2_push.x)
	//		{
	//			if (go->position.y > go2->position.y)
	//			{
	//				go->velocity.y = 0;
	//			}
	//			else
	//			{
	//				go2->velocity.y = 0;
	//			}
	//		}
	//		else
	//		{
	//			go->velocity += go1_push;
	//			go2->velocity += go2_push;
	//		}*/
	//	}
	//}
}

void PhysicsManager::Render()
{
	qtGameObjects->RenderDebug();
}

void PhysicsManager::End()
{
	gameObjects.clear();
	if (qtGameObjects != nullptr)
	{
		delete qtGameObjects;
	}
}

void PhysicsManager::PushGameObject(GameObjectEntity* gameObject)
{
	gameObjects.push_back(gameObject);
}