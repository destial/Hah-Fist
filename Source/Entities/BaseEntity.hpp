/*!
* @file BaseEntity.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
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
#include <cmath>
#include "AEEngine.h"
#include "../Utils/Utils.hpp"
#include "../Items/Image.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Utils/AEOverload.hpp"

/*!
* @brief The base entity class that should be inherited by all other entities
* @brief All UI, enemies, player, etc. anything in the scene, is derived from
* @brief this class.
*/
class BaseEntity {
private:
	static s32 all_id; // Current static id when assigning
	s32 en_id; // This entity's unique id
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

	// Sprite animation data
	f32 animation_timer{ 0.f }, animation_frame{ 0.f };
	int current_row{ 0 }, current_col{ 0 }, max_rows{ 1 }, max_columns{ 1 };
	SpriteSheet* sprite{ nullptr };
public:
	virtual ~BaseEntity(); // Dtor
	AEGfxVertexList* mesh; // Mesh data for rendering
	Image* image; // Image data for rendering
	AEVec2 position; // World position
	AEVec2 velocity; // Relative world velocity
	AEVec2 scale; // World scaling
	f32 rotation; // In radians
	Color color; // Blend mesh color for rendering
	enum struct RenderLayer : int {
		BACKGROUND = -5, WORLD = 0, ENTITY = 1, PLAYER = 2, WORLD_UI = 4, UI = 5
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
	* @return If there were listeners removed
	*/
	bool RemoveUpdateListener(void* owner);

	/*!
	* @brief Remove all owned functions from this entity's PreUpdate
	* @param owner - The owner of the functions
	* @return If there were listeners removed
	*/
	bool RemovePreUpdateListener(void* owner);

	/*!
	* @brief Remove all owned functions from this entity's PostUpdate
	* @param owner - The owner of the functions
	* @return If there were listeners removed
	*/
	bool RemovePostUpdateListener(void* owner);

	/*!
	* @brief Initializes animated sprite data for this entity, use InitializeSpriteData if
	* @brief you want to initialize a non animated sprite.
	* @param filepath - Filepath to the sprite, be sure to use AssetManager macros for this.
	* @param rows - number of rows in the spritesheet (Rows are used for different animations).
	* @param columns - number of columns in the spritesheet (Columns are the number of frames).
	*/
	void InitializeAnimatedSpriteData(std::string filepath, int rows, int columns, f32 _scale = 5.f);

	/*!
	* @brief Initializes sprite data for this entity, use InitializeAnimatedSpriteData if
	* @brief you want to initialize an animated sprite.
	* @param filepath - Filepath to the sprite, be sure to use AssetManager macros for this.
	*/
	void InitializeSpriteData(std::string filepath, f32 _scale = 5.f);

	/*!
	* @brief Get the half-size of this entity
	* @return The absolute scale * 0.5
	*/
	inline AEVec2 GetHalfSize() const { return AEVec2{ std::abs(scale.x), std::abs(scale.y) } *0.5f; }

	/*!
	* @brief Get the unique id of this entity
	* @return The unique id
	*/
	inline s32 GetId() const { return en_id; }
};

#endif