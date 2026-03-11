/*!
* @file GameScene.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 7 January 2026
* @course CSD1451
* @brief Declaration file for a base scene that will be inherited for all scenes
*/

#pragma once
#ifndef __GAMESCENE_H
#define __GAMESCENE_H
#include "BaseScene.hpp"

/*!
* @brief Game scene class
* @brief Contains the level data, player, enemies, etc.
*/
class GameScene : public BaseScene {
protected:
	f32 game_timer;
public:
	GameScene(); // Ctor
	GameScene(GameScene const&) = delete; // Remove copy ctor
	GameScene& operator=(GameScene const&) = delete; // Remove copy assignment
	virtual ~GameScene(); // Dtor

	/*!
	* @brief Inherited: Initialize the scene with starting points, entities, level, etc.
	*/
	virtual void Init();

	void Update(const f32& dt) override;

	/*!
	* @brief Inherited: Clear the entities from the scene.
	*/
	virtual void End();

	void Win();

	void Lose();
};

#endif
