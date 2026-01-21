#pragma once
#ifndef __PLAYERENTITY_H
#define __PLAYERENTITY_H
#include "GameObjectEntity.hpp"
#include "../Items/SpriteSheet.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/PhysicsBody.hpp"

class Player : public GameObjectEntity {
protected:
	f32 animationTimer, animationFrame;
	f32 jumpHeight;
	f32 jumpVelocity;
	f32 speed;
	int currentRow, currentCol;
	SpriteSheet* sprite;
public:
	Player(AEVec2 pos = { 0.f, 0.f });
	virtual ~Player();
	PhysicsBody* pBody;
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};

#endif