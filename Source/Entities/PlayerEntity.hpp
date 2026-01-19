#pragma once
#ifndef __PLAYERENTITY_H
#define __PLAYERENTITY_H
#include "BaseEntity.hpp"
#include "../Items/SpriteAnimation.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/PhysicsBody.hpp"

class Player : public BaseEntity {
protected:
	f32 animationTimer, animationFrame;
	f32 jumpHeight;
	f32 jumpVelocity;
	f32 speed;
	int currentRow, currentCol;
	SpriteAnimation* sprite;
public:
	Player();
	Player(AEVec2 pos);
	~Player();
	PhysicsBody* pBody;
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};

#endif