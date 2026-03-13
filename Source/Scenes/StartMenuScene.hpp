/*!
* @file StartMenuScene.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 26 January 2026
* @course CSD1451
* @brief Declaration file for a start menu scene
*/

#pragma once
#ifndef __STARTMENUSCENE_H
#define __STARTMENUSCENE_H
#include "BaseScene.hpp"

/*!
* @brief Start menu scene class
* @brief Display the play, quit and credits buttons
* @brief Transitions into the Game scene or Credits scene or quit
*/
class StartMenuScene : public BaseScene {
public:
	StartMenuScene(); // Ctor
	StartMenuScene(StartMenuScene const&) = delete; // Remove copy ctor
	StartMenuScene& operator=(StartMenuScene const&) = delete; // Remove copy assignment
	virtual ~StartMenuScene(); // Dtor

	/*!
	* @brief Inherited: Initialize the scene with UI starting points
	*/
	void Init() override;
};

#endif
