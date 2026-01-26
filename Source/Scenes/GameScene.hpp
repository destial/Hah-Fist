#pragma once
#ifndef __GAMESCENE_H
#define __GAMESCENE_H
#include "BaseScene.hpp"
#include "../Entities/GameObjectEntity.hpp"
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

	std::vector<GameObjectEntity*> gameObjects;
};

#endif
