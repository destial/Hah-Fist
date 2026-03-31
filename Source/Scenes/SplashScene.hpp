/*!
* @file SplashScene.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Declaration file for a splash screen scene
*/

#pragma once
#ifndef __SPLASHSCREEN_H
#define __SPLASHSCREEN_H
#include "BaseScene.hpp"

/*!
* @brief Splashscreen scene class
* @brief Display's the splash screen for 2 seconds
* @brief then transitions into the main menu
*/
class SplashScene : public BaseScene {
private:
	f32 timer{ 0.f };
public:
	SplashScene(); // Ctor
	SplashScene(SplashScene const&) = delete; // Remove copy ctor
	SplashScene& operator=(SplashScene const&) = delete; // Remove copy assignment
	virtual ~SplashScene(); // Dtor

	/*!
	* @brief Inherited: Initialize the scene with the splash screen image
	*/
	void Init() override;

	/*!
	* @brief Inherited: Update all entities in the scene
	* @param dt - The delta time for this frame
	*/
	void Update(const f32& dt) override;
};

#endif