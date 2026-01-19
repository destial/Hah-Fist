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
	bool isActive;
public:
	GameObjectEntity();
	//~GameObjectEntity();
	

	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};
#endif