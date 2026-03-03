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

class GameScene : public BaseScene {
public:
	GameScene();
	~GameScene();
	virtual void Init();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void End();
};

#endif
