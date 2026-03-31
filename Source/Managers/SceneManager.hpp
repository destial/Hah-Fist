/*!
* @file SceneManager.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 7 January 2026
* @course CSD1451
* @brief Declaration file for a scene manager that handles 
		 processing and transition of different scenes
*/

#pragma once
#ifndef __SCENEMANAGER_H
#define __SCENEMANAGER_H
#include "AETypes.h"
#include "../Scenes/BaseScene.hpp"
#include "../Utils/Singleton.hpp"
#include "LevelEditor.hpp"

namespace Scenes { // start scene namespace
	enum SceneState { // declare available scenes
		INIT = -1, // this is only to declare the initial scene
		SPLASH_SCREEN = 0,
		MAIN_MENU,
		GAME,
		CREDITS,
		LAST, // to count the total number of scenes
	};
} // end scene namespace

/*!
* @brief Singleton class that handles all scene processing
*/
class SceneManager : public Singleton<SceneManager> {
protected:
	BaseScene* all_scenes[Scenes::LAST]; // array of all scenes

	Scenes::SceneState next_scene; // the next scene to play the next frame
	Scenes::SceneState current_scene; // the current scene playing

	LevelEditor* editor; // the level editor

public:
	SceneManager(); // Ctor
	~SceneManager(); // Dtor

	/*!
	* @brief Called before InputManager->Update()
	*		 Process anything before the main Update()
	*/
	void PreUpdate(const f32& dt);

	/*!
	* @brief Called after InputManager->Update()
	*		 Process input, physics, logic
	*/
	void Update(const f32& dt);

	/*!
	* @brief Called after SceneManager->Update()
	*		 Process transform and other calculations
	*/
	void PostUpdate(const f32& dt);

	/*!
	* @brief Render to the screen
	*/
	void Render();

	/*!
	* @brief Set the next scene to be played at the next frame
	* @param next - The next scene to be played
	*/
	void SetNextScene(Scenes::SceneState next);

	/*!
	* @brief Get the current scene
	* @return The currently played scene
	*/
	BaseScene* GetCurrentScene() const;

	/*!
	* @brief Get the current scene state
	* @return The currently played scene state
	*/
	Scenes::SceneState GetCurrentState() const;

	/*!
	* @brief Get LevelEditor pointer
	* @return The pointer to the LevelEditor
	*/
	LevelEditor* GetEditor() const;
};

#endif