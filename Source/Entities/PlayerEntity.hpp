#pragma once
#ifndef __PLAYERENTITY_H
#define __PLAYERENTITY_H
#include "BaseEntity.hpp"

class Player : public BaseEntity {
public:
	Player(AEVec2 pos);
	~Player();

	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
};

#endif