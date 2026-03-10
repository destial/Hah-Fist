/*!
* @file CreditsScene.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Declaration file for a credits scene
*/

#pragma once
#ifndef __CREDITS_SCENE_H
#define __CREDITS_SCENE_H
#include "BaseScene.hpp"

/*!
* @brief Credits scene class
* @brief Display's the credit scene for 2 seconds
* @brief then transitions back into the main menu
*/
class CreditsScene : public BaseScene {
public:
	CreditsScene(); // Ctor
	CreditsScene(CreditsScene const&) = delete; // Remove copy ctor
	CreditsScene& operator=(CreditsScene const&) = delete; // Remove copy assignment
	virtual ~CreditsScene(); // Dtor

	/*!
	* @brief Inherited: Initialize the scene with the credits scene image
	*/
	virtual void Init();

	/*!
	* @brief Inherited: Update all entities in the scene
	* @param dt - The delta time for this frame
	*/
	virtual void Update(const f32& dt);
};

#endif