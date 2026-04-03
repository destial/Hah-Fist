/*!
* @file BaseEntity.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 9 January 2026
* @course CSD1451
* @brief Definition file for a base entity that will be inherited for all entities
*/

#include "BaseEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"

s32 BaseEntity::all_id{}; // Static

BaseEntity::BaseEntity(AEVec2 position) :
	position{ position },
	velocity{ 0 },
	scale{ 1.f, 1.f },
	rotation{ 0 },
	transform{ 0 },
	layer{RenderLayer::WORLD},
	mesh{ nullptr }, 
	image{ nullptr }, 
	color{ 255, 255, 255, 255 },
	en_id{all_id++}
{
	// Empty ctor body
}

BaseEntity::~BaseEntity() {
	mesh = nullptr;
	image = nullptr;
	for (auto& entry : update_listeners) {
		entry.second.clear(); // Clear all update listeners
	}
	update_listeners.clear();
	for (auto& entry : preupdate_listeners) {
		entry.second.clear(); // Clear all pre-update listeners
	}
	preupdate_listeners.clear();
	for (auto& entry : postupdate_listeners) {
		entry.second.clear(); // Clear all post-update listeners
	}
	postupdate_listeners.clear();
}

/*!
* @brief Pre-update the entity, before any input has been processed
*/
void BaseEntity::PreUpdate(const f32& dt) {
	for (auto& entry : preupdate_listeners) {
		for (auto& func : entry.second) {
			func(dt);
		}
	}
}

/*!
* @brief Update the entity after input has been processed
*/
void BaseEntity::Update(const f32& dt) {
	for (auto& entry : update_listeners) {
		for (auto& func : entry.second) {
			func(dt);
		}
	}
}

/*!
* @brief Post-update the entity after everything else has been processed
*/
void BaseEntity::PostUpdate(const f32& dt) {
	for (auto& entry : postupdate_listeners) {
		for (auto& func : entry.second) {
			func(dt);
		}
	}

	// Generic Animation Functionality across all entities
	if (maxColumns > 1) {
		if ((animation_timer += dt) > animation_frame) {
			animation_timer = 0.f;
			if (++current_col >= maxColumns) {
				current_col = 0;
			}
		}
	}
	
	// Calculate transformation matrix for rendering
	transform = Utils::GetTransformMatrix(position, scale, rotation);
}

/*!
* @brief Render the entity to the screen
*/
void BaseEntity::Render() {
	// In-case post-update was skipped, calculate the transform again
	transform = Utils::GetTransformMatrix(position, scale, rotation);

	if (sprite != nullptr) { // Render the sprite if it exists
		sprite->Render(transform, color, current_row, current_col);
	}

	if (!mesh)
		return; // Nothing to render

	// Use image texture data if exists
	if (image && image->data) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(image->data, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(color.r / 255.f, color.b / 255.f, color.g / 255.f, color.a / 255.f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(color.a / 255.f);

	// Apply transform and draw the mesh, resetting the global texture data after
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
}

/*!
* @brief Add a function to this entity's Update
* @param owner - The owner to the function
* @param func - The function to call
*/
void BaseEntity::AddUpdateListener(void* owner, std::function<void(const f32&)> func) {
	update_listeners[owner].push_back(func);
}

/*!
* @brief Add a function to this entity's PreUpdate
* @param owner - The owner to the function
* @param func - The function to call
*/
void BaseEntity::AddPreUpdateListener(void* owner, std::function<void(const f32&)> func) {
	preupdate_listeners[owner].push_back(func);
}

/*!
* @brief Add a function to this entity's PostUpdate
* @param owner - The owner to the function
* @param func - The function to call
*/
void BaseEntity::AddPostUpdateListener(void* owner, std::function<void(const f32&)> func) {
	postupdate_listeners[owner].push_back(func);
}

/*!
* @brief Remove all owned functions from this entity's Update
* @param owner - The owner of the functions
*/
bool BaseEntity::RemoveUpdateListener(void* owner) {
	if (update_listeners.count(owner)) { // If this owner has listeners, remove all listeners from this owner
		update_listeners[owner].clear();
		update_listeners.erase(owner);
		return true;
	}
	return false;
}

/*!
* @brief Remove all owned functions from this entity's PreUpdate
* @param owner - The owner of the functions
* @return If there were listeners removed
*/
bool BaseEntity::RemovePreUpdateListener(void* owner) {
	if (preupdate_listeners.count(owner)) { // If this owner has listeners, remove all listeners from this owner
		preupdate_listeners[owner].clear();
		preupdate_listeners.erase(owner);
		return true;
	}
	return false;
}

/*!
* @brief Remove all owned functions from this entity's PostUpdate
* @param owner - The owner of the functions
* @return If there were listeners removed
*/
bool BaseEntity::RemovePostUpdateListener(void* owner) {
	if (postupdate_listeners.count(owner)) { // If this owner has listeners, remove all listeners from this owner
		postupdate_listeners[owner].clear();
		postupdate_listeners.erase(owner);
		return true;
	}
	return false;
}

/*!
* @brief Initializes animated sprite data for this entity, use InitializeSpriteData if
* @brief you want to initialize a non animated sprite.
* @param filepath - Filepath to the sprite, be sure to use AssetManager macros for this.
* @param rows - number of rows in the spritesheet (Rows are used for different animations).
* @param columns - number of columns in the spritesheet (Columns are the number of frames).
*/
void BaseEntity::InitializeAnimatedSpriteData(std::string filepath, int rows, int columns, f32 _scale) {
	maxRows = rows;
	maxColumns = columns;
	InitializeSpriteData(filepath, _scale);
	animation_timer = 0.f;
	animation_frame = 1.f / static_cast<f32>(maxColumns * maxRows);
}

/*!
* @brief Initializes sprite data for this entity, use InitializeAnimatedSpriteData if
* @brief you want to initialize an animated sprite.
* @param filepath - Filepath to the sprite, be sure to use AssetManager macros for this.
*/
void BaseEntity::InitializeSpriteData(std::string filepath, f32 _scale) {
	sprite = AssetManager::GetSpriteSheet(filepath, maxRows, maxColumns);
	mesh = nullptr;
	current_row = current_col = 0;
	scale = { _scale * ((static_cast<f32>(sprite->image->width) / static_cast<f32>(maxColumns)) / (sprite->image->height / static_cast<f32>(maxRows))) , _scale };
}