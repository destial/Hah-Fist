#pragma once
#ifndef __ENEMYENTITY_H
#define __ENEMYENTITY_H
#include "GameObjectEntity.hpp"
#include "../Items/SpriteSheet.hpp"

class EnemyEntity : public GameObjectEntity {
protected:
	f32 animationTimer, animationFrame;
	int currentRow, currentCol;
	SpriteSheet* sprite;
public:
	EnemyEntity(AEVec2 pos);
	virtual ~EnemyEntity();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};
#endif