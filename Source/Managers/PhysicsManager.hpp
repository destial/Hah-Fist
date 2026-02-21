#pragma once
#ifndef __PHYSICSMANAGER_H
#define __PHYSICSMANAGER_H

#include "AEEngine.h"
#include <vector>
#include "../Utils/QuadTree.hpp"

class PhysicsManager {
private:
	Physics::AABB worldBounds{ AEVec2{-500.0f, -500.0f}, AEVec2{500.0f, 500.0f} };
	size_t maxEntriesPerNode{ 1 };
	QuadTree::Tree* qtGameObjects{ nullptr };
public:
	//PhysicsManager();
	//~PhysicsManager();
	std::vector<GameObjectEntity*> gameObjects;
	void Init();
	void PreUpdate(const f32& dt);
	void Update(const f32& dt);
	void Render();
	void End();

	void PushGameObject(GameObjectEntity* gameObject);
};

#endif
