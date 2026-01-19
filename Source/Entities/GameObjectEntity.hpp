#pragma once
#pragma once
#ifndef __GAMEOBJECTENTITY_H
#define __GAMEOBJECTENTITY_H
#include "BaseEntity.hpp"
class GameObjectEntity : public BaseEntity {
protected:
	enum SHAPE {
		CIRCLE,
		AABB,
		TOTAL
	}type;
	f32 health;
	f32 damage;
	bool isActive;
public:
	GameObjectEntity();
	virtual ~GameObjectEntity();
	

	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};
#endif