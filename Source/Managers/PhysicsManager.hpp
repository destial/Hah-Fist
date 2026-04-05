/*!
* @file PhysicsManager.hpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @date 22nd February 2026
* @course CSD1451
* @brief This source file declares the PhysicsManager class and its member functions, which
* handles physics for all objects within the gameplay loop.
*/
#pragma once
#ifndef __PHYSICSMANAGER_H
#define __PHYSICSMANAGER_H

#include "AEEngine.h"
#include <vector>
#include "../Utils/QuadTree.hpp"

class PhysicsManager {
private:
	Physics::AABB world_bounds; //Bounds of the world
	size_t max_entries_per_node{ 1 }; //maximum entries per node for the QuadTree
	QuadTree::Tree* qt_game_objects{ nullptr }; //Quadtree
public:
	/*!
	* @brief Constructor function for a PhysicsManager. Initializes the basic data for the PhysicsManager to operate
	* such as the world size, maximum entries per node for the QuadTree, and the zeroed vector of gameobjects.
	* @param Physics::AABB _world_bounds - Max size of the world.
	* @param size_t _max_entries_per_node - Max objects within each node of the QuadTree.
	*/
	PhysicsManager(Physics::AABB _world_bounds = { AEVec2{-500.0f, -500.0f}, AEVec2{500.0f, 500.0f} }, size_t _max_entries_per_node = 1);

	/*!
	* @brief Destructor function for a PhysicsManager.
	*/
	~PhysicsManager();
	std::vector<GameObjectEntity*> game_objects;
	void Init();

	/*!
	* @brief PostUpdate for a PhysicsManager.
	* Manages the collision detections and responses of all GameObjectEntities
	* @param const f32& dt - delta time
	*/
	void PostUpdate(const f32& dt);

	/*!
	* @brief Render function for a PhysicsManager.
	* Renders debugging information for the PhysicsManager
	*/
	void Render();

	/*!
	* @brief Clears the runtime information from a PhysicsManager
	*/
	void Clear();

	/*!
	* @brief Clears the runtime information from a PhysicsManager
	* @param GameObjectEntity* gameObject - Adds a GameObject to be handled by this PhysicsManager
	*/
	void PushGameObject(GameObjectEntity* game_object);

	/*!
	* @brief Handles the collision response between static and dynamic objects
	* @param GameObjectEntity* _static - static object in the collision
	* @param GameObjectEntity* _dynamic - dynamic object in the collision
	*/
	void HandleStaticDynamicCollisionResponse(GameObjectEntity* _static, GameObjectEntity* _dynamic);

	/*!
	* @brief Handles the collision response between dynamic objects
	* @param GameObjectEntity* first - first dynamic object in the collision
	* @param GameObjectEntity* second - second dynamic object in the collision
	*/
	void HandleDynamicDynamicCollisionResponse(GameObjectEntity* first, GameObjectEntity* second);

	/*!
	* @brief Gets a list of collision targets.
	* @param GameObjectEntity* first - dynamic object to poll with other objects
	* @param std::vector<GameObjectEntity*> ignored - objects to ignore
	* @param GameObjectEntity::PhysicsType type_filter - filter for the types of objects
	*/
	std::vector<GameObjectEntity*> GetPotentialCollisionTargets(GameObjectEntity* first, std::vector<GameObjectEntity*> ignored, GameObjectEntity::PhysicsType type_filter = GameObjectEntity::PhysicsType::TOTAL) const;
};

#endif
