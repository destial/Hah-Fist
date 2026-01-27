#pragma once
#ifndef __ENEMYENTITY_H
#define __ENEMYENTITY_H
#include "GameObjectEntity.hpp"

class EnemyEntity : public GameObjectEntity {
public:
	EnemyEntity(AEVec2 pos);
	virtual ~EnemyEntity();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};
#endif