#pragma once
#ifndef __PLAYERENTITY_H
#define __PLAYERENTITY_H
#include "BaseEntity.hpp"
#include "../Items/SpriteAnimation.hpp"
#include "../Events/InputEvent.hpp"

class Player : public BaseEntity {
protected:
	f32 animationTimer, animationFrame;
	int currentRow, currentCol;
	SpriteAnimation* sprite;
public:
	Player();
	Player(AEVec2 pos);
	~Player();

	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};

#endif