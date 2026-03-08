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
* @brief Base scene class
*		 Every scene should have its entities, particle system,
*		 physics manager and camera manager
*/
class BaseScene {
private:
	std::vector<BaseEntity*> awaiting_deletion;
protected:
	std::vector<BaseEntity*> scene_entities; // vector of entity pointers
	ParticleSystem* particleSystem; // pointer to its own particle system
	PhysicsManager* physicsManager{ nullptr }; // pointer to its own physics manager
	CameraManager* camManager; // pointer to its own camera manager
	BaseScene(); // Ctor (protected here so cant be called outside of inherited classes)

public:
	virtual ~BaseScene(); // Dtor, calls delete for all entities in the scene

	/*!
	* @brief Initialize the scene with starting points, entities, level, etc.
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

	std::vector<BaseEntity*> const& Entities() const;

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
	template<typename E> std::vector<E*> GetEntitesOfType() const {
		static_assert(std::is_base_of<BaseEntity, E>::value, "E must derive from BaseEntity!");
		std::vector<E*> vect;
		for (BaseEntity* en : scene_entities) {
			if (E* type = dynamic_cast<E*>(en)) {
				vect.push_back(type);
			}
		}
		return vect;
	}
};

#endif
