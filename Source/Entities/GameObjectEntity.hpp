#pragma once
#pragma once
#ifndef __GAMEOBJECTENTITY_H
#define __GAMEOBJECTENTITY_H
#include "BaseEntity.hpp"
#include "../Utils/PhysicsBody.hpp"

class GameObjectEntity : public BaseEntity {
protected:
	f32 health;
	f32 damage;
public:
	PhysicsBody* pBody;
	enum SHAPE {
		CIRCLE,
		AABB,
		TOTAL
	}type;
	enum KINEMATIC {
		STATIC,
		DYNAMIC
	}go_type;
	GameObjectEntity();
	GameObjectEntity(AEVec2 pos = { 0.f, 0.f }, f32 mass = 1.0f, SHAPE type = AABB);
	virtual ~GameObjectEntity();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	bool isActive;
};
#endif