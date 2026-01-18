#pragma once
#pragma once
#ifndef __GAMEOBJECTENTITY_H
#define __GAMEOBJECTENTITY_H
#include "BaseEntity.hpp"
class GameObjectEntity : public BaseEntity {
public:
	GameObjectEntity();
	//~GameObjectEntity();
	bool isActive;
};
#endif