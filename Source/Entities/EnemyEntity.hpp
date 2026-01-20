#pragma once
#include "GameObjectEntity.hpp"
#include "../Utils/PhysicsBody.hpp"
#ifndef __ENEMYENTITY_H
#define __ENEMYENTITY_H
class EnemyEntity : public GameObjectEntity {
public:
	EnemyEntity();
	EnemyEntity(AEVec2 pos);
	virtual ~EnemyEntity();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();

	PhysicsBody* pBody;
};
#endif