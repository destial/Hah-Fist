/*!
* @file BaseScene.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 7 January 2026
* @course CSD1451
* @brief Declaration file for a base scene that will be inherited for all scenes
*/

#pragma once
#ifndef __BASESCENE_H
#define __BASESCENE_H
#include <vector>
#include <memory>
#include "AETypes.h"
#include "../Entities/BaseEntity.hpp"
#include "../Items/ParticleSystem.hpp"
#include "../Managers/PhysicsManager.hpp"
#include "../Managers/CameraManager.hpp"

/*!
* @brief An enum of the scene frame state
*/
enum struct FrameState {
	INIT, PRE, MIDDLE, POST, RENDER, END
};

/*!
* @brief Base scene class
* @brief Every scene should have its entities, particle system,
* @brief physics manager and camera manager
*/
class BaseScene {
private:
	FrameState current_state; // the current frame state of the code execution
	std::vector<BaseEntity*> awaiting_deletion; // all the entities that are waiting to be removed
	std::map<BaseEntity*, BaseEntity*> linked_entities; // entities that are linked to other entities
protected:
	std::vector<BaseEntity*> scene_entities; // vector of entity pointers
	ParticleSystem* particleSystem; // pointer to its own particle system
	PhysicsManager* physicsManager{ nullptr }; // pointer to its own physics manager
	CameraManager* camManager; // pointer to the singleton camera manager
	BaseScene(); // Ctor (protected here so cant be called outside of inherited classes)
	BaseScene(BaseScene const&) = delete; // Delete copy ctor
	BaseScene& operator=(BaseScene const&) = delete; // Delete copy assignment

	/*!
	* @brief Internal function to delete an entity's allocated memory from the scene
	* @param entity - The entity to be deleted in this function call
	*/
	void DeleteEntityFromScene(BaseEntity* entity);
public:
	virtual ~BaseScene(); // Dtor, calls delete for all entities in the scene

	/*!
	* @brief Initialize the scene with
	*/
	virtual void Init() = 0;

	/*!
	* @brief PreUpdate all entities in the scene
	* @param dt - The delta time for this frame
	*/
	virtual void PreUpdate(const f32& dt);

	/*!
	* @brief Update all entities in the scene
	* @param dt - The delta time for this frame
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief PostUpdate all entities in the scene
	* @param dt - The delta time for this frame
	*/
	virtual void PostUpdate(const f32& dt);

	/*!
	* @brief Render all active entities in the scene
	*/
	virtual void Render();

	/*!
	* @brief Called before transitioning to a different scene
	*/
	virtual void End();

	/*!
	* @brief Add a new entity to the scene
	*		 Does NOT check for unique pointers
	* @param entity - The entity to add
	*/
	void AddEntityToScene(BaseEntity* entity);

	/*!
	* @brief Removes an entity from the scene
	* @param entity - The entity to remove
	*/
	void RemoveEntityFromScene(BaseEntity* entity);

	/*!
	* @brief Get a constant reference to all the entities in the scene
	* @return A constant vector reference to the scene entities
	*/
	std::vector<BaseEntity*> const& Entities() const;

	/*!
	* @brief Get the current frame state (INIT, PRE_UPDATE, UPDATE, POST_UPDATE, RENDER, END)
	* @brief of this scene at the point this code has been called.
	* @return An enum of the current frame state
	*/
	FrameState GetFrameState() const;

	/*!
	* @brief Get the first entity of this class type
	* @return The first entity pointer of this class, else nullptr
	*/
	template<typename E> E* GetFirstEntityOfType() const {
		static_assert(std::is_base_of<BaseEntity, E>::value, "E must derive from BaseEntity!");
		for (BaseEntity* en : scene_entities) {
			if (E* first = dynamic_cast<E*>(en)) {
				return first;
			}
		}
		return nullptr;
	}

	/*!
	* @brief Get all entities of this class type
	* @return A vector of entity pointers of this class
	*/
	template<typename E> std::vector<E*> GetEntitiesOfType() const {
		static_assert(std::is_base_of<BaseEntity, E>::value, "E must derive from BaseEntity!");
		std::vector<E*> vect;
		for (BaseEntity* en : scene_entities) {
			if (E* type = dynamic_cast<E*>(en)) {
				vect.push_back(type);
			}
		}
		return vect;
	}

	template<typename E> std::vector<BaseEntity*> GetBaseEntitiesOfType() const {
		static_assert(std::is_base_of<BaseEntity, E>::value, "E must derive from BaseEntity!");
		std::vector<BaseEntity*> vect;
		for (BaseEntity* en : scene_entities) {
			if (E* type = dynamic_cast<E*>(en)) {
				vect.push_back(type);
			}
		}
		return vect;
	}
};

#endif
