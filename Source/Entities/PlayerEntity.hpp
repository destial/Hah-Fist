#pragma once
#ifndef __PLAYERENTITY_H
#define __PLAYERENTITY_H
#include "BaseEntity.hpp"

class Player : public BaseEntity {
public:
	Player(AEVec2 pos);
	~Player();
};

#endif