/*!
* @file BaseEntity.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 9 January 2026
* @course CSD1451
* @brief Declaration file for a base entity that will be inherited for all entities
*/

#pragma once
#ifndef __BASEENTITY_H
#define __BASEENTITY_H
#include <vector>
#include <map>
#include <functional>
#include "AEEngine.h"
#include "../Utils/Utils.hpp"
#include "../Items/Image.hpp"

/*!
* @brief The base entity class that should be inherited by all other entities
* @brief All UI, enemies, player, etc. anything in the scene, is derived from
* @brief this class.
*/
class BaseEntity {
protected:
	// Only encourage the use of heap instantiation (pointers)
	BaseEntity(AEVec2 position = { 0.f, 0.f }); // Ctor
	BaseEntity(BaseEntity const&) = delete; // Remove copy ctor
	BaseEntity& operator=(BaseEntity const&) = delete; // Remove copy assignment
	AEMtx33 transform; // Transformation matrix for rendering

	// Functions to call in the entity PreUpdate
	std::map<void*, std::vector<std::function<void(const f32&)>>> preupdate_listeners;

	// Functions to call in the entity Update
	std::map<void*, std::vector<std::function<void(const f32&)>>> update_listeners;

	// Functions to call in the entity PostUpdate
	std::map<void*, std::vector<std::function<void(const f32&)>>> postupdate_listeners;

public:
	virtual ~BaseEntity(); // Dtor
	AEGfxVertexList* mesh; // Mesh data for rendering
	Image* image; // Image data for rendering
	AEVec2 position; // World position
	AEVec2 velocity; // Relative world velocity
	AEVec2 scale; // World scaling
	f32 rotation; // In radians
	Color color; // Blend mesh color for rendering
	enum struct RenderLayer {
		WORLD = 0, ENTITY = 1, PLAYER = 2, UI = 5
	} layer; // Rendering layer for Z-layering

	/*!
	* @brief Pre-update the entity, before any input has been processed
	*/
	virtual void PreUpdate(const f32& dt);

	/*!
	* @brief Update the entity after input has been processed
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Post-update the entity after everything else has been processed
	*/
	virtual void PostUpdate(const f32& dt);

	/*!
	* @brief Render the entity to the screen
	*/
	virtual void Render();

	/*!
	* @brief Add a function to this entity's Update
	* @param owner - The owner to the function
	* @param func - The function to call
	*/
	void AddUpdateListener(void* owner, std::function<void(const f32&)> func);

	/*!
	* @brief Add a function to this entity's PreUpdate
	* @param owner - The owner to the function
	* @param func - The function to call
	*/
	void AddPreUpdateListener(void* owner, std::function<void(const f32&)> func);

	/*!
	* @brief Add a function to this entity's PostUpdate
	* @param owner - The owner to the function
	* @param func - The function to call
	*/
	void AddPostUpdateListener(void* owner, std::function<void(const f32&)> func);

	/*!
	* @brief Remove all owned functions from this entity's Update
	* @param owner - The owner of the functions
	*/
	bool RemoveUpdateListener(void* owner);

	/*!
	* @brief Remove all owned functions from this entity's PreUpdate
	* @param owner - The owner of the functions
	*/
	bool RemovePreUpdateListener(void* owner);

	/*!
	* @brief Remove all owned functions from this entity's PostUpdate
	* @param owner - The owner of the functions
	*/
	bool RemovePostUpdateListener(void* owner);
};

#endif