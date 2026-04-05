#pragma once
#ifndef __PHYSICSMANAGER_H
#define __PHYSICSMANAGER_H

#include "AEEngine.h"
#include <vector>
#include "../Utils/QuadTree.hpp"

class PhysicsManager {
private:
	Physics::AABB world_bounds;
	size_t max_entries_per_node{ 1 };
	QuadTree::Tree* qt_game_objects{ nullptr };
public:
	PhysicsManager(Physics::AABB _world_bounds = { AEVec2{-500.0f, -500.0f}, AEVec2{500.0f, 500.0f} }, size_t _max_entries_per_node = 1);
	~PhysicsManager();
	std::vector<GameObjectEntity*> game_objects;
	void Init();
	void PreUpdate(const f32& dt);
	void PostUpdate(const f32& dt);
	void Render();
	void Clear();

	void PushGameObject(GameObjectEntity* game_object);

	void HandleStaticDynamicCollisionResponse(GameObjectEntity* _static, GameObjectEntity* _dynamic);
	void HandleDynamicDynamicCollisionResponse(GameObjectEntity* first, GameObjectEntity* second);

	std::vector<GameObjectEntity*> GetPotentialCollisionTargets(GameObjectEntity* first, std::vector<GameObjectEntity*> ignored, GameObjectEntity::PhysicsType type_filter = GameObjectEntity::PhysicsType::TOTAL) const;
};

#endif
