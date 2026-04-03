/*!
* @file LevelEditor.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 23 February 2026
* @course CSD1451
* @brief Declaration file for a level editor that the user
* @brief can actively modify and save the entities inside a level
*/

#pragma once
#ifndef __LEVEL_EDITOR_H
#define __LEVEL_EDITOR_H
#include "AEEngine.h"
#include "../Scenes/BaseScene.hpp"
#include <vector>

namespace Editor { // start namespace editor
	/*!
	* @brief I don't have a cleaner way of adding what
	* @brief entities I can save, so enum is the best
	* @brief option I have currently
	*/
	enum GameObjectType {
		STATIC_PLATFORM,
		STATIC_WALL,
		ENEMY_1,
		ENEMY_2,
		PROJECTILE_ENEMY,
		COIN,
		CRATE,
		SPIDER,
		TITAN,
		PAYLOAD,
		IRONSIDE,
		MOVING_PLATFORM,
		BOSS_SPAWN_WALL,
	};
}; // end namespace editor

/*!
* @brief Stores the editing and selection entity objects
* @brief as well as how to manipulate them inside the level
*/
class LevelEditor {
private:
	bool toggled; // Toggle flag
	BaseScene* scene; // The current editing scene
	std::vector<std::pair<BaseEntity*, AEVec2>> current_selections; // The current entity selected
public:
	LevelEditor(BaseScene* scene); // Ctor
	LevelEditor(LevelEditor const&) = delete; // Remove copy ctor
	LevelEditor& operator=(LevelEditor const&) = delete; // Remove copy assignment
	~LevelEditor(); // Dtor

	/*!
	* @brief Toggle the level editor state
	*/
	void Toggle();

	/*!
	* @brief Get the current state of the level editor
	*/
	bool IsToggled() const;

	/*!
	* @brief Set the current scene to edit
	* @param scene - The scene to edit, called when transitioning
	*/
	void SetScene(BaseScene* scene);

	/*!
	* @brief Set the current entity selection
	* @param entity - The entity to select
	*/
	void SelectEntity(BaseEntity* entity);

	/*!
	* @brief Remove the current entity selection from the scene
	* @brief This is safe from concurrent modification
	*/
	void RemoveSelectedEntity();

	/*!
	* @brief Add an entity to the scene based on the GameObject type
	*/
	BaseEntity* AddEntity(Editor::GameObjectType type);

	/*!
	* @brief Update the level editor per frame, to manipulate
	* @brief the currently selected entity
	*/
	void Update(const f32& dt);
	
	/*!
	* @brief Render data about the level, as well as if the level
	* @brief has been saved or not
	*/
	void Render();
};

#endif
