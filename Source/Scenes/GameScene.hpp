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
	static std::vector<BaseEntity*> staticEntities;
public:
	GameScene(); // Ctor
	GameScene(GameScene const&) = delete; // Remove copy ctor
	GameScene& operator=(GameScene const&) = delete; // Remove copy assignment
	virtual ~GameScene(); // Dtor

	/*!
	* @brief Inherited: Initialize the scene with starting points, entities, level, etc.
	*/
	void Init() override;

	/*!
	* @brief Inherited: Update all entities in the scene
	* @param dt - The delta time for this frame
	*/
	void Update(const f32& dt) override;

	/*!
	* @brief Inherited: PostUpdate all entities in the scene
	* @param dt - The delta time for this frame
	*/
	void PostUpdate(const f32& dt) override;

	/*!
	* @brief Inherited: Clear the entities from the scene.
	*/
	void End() override;

	/*!
	* @brief Called when the current level has been won
	*/
	void Win();

	/*!
	* @brief Called when the current level has been lost
	*/
	void Lose();

	static std::vector<BaseEntity*>& GetStaticEntities();
};

#endif
