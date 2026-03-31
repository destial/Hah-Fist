#pragma once
#ifndef __LASER_ENTITY_H
#define __LASER_ENTITY_H
#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"

class LaserEntity : public GameObjectEntity {
protected:
	GameObjectEntity* owner;
	GameObjectEntity* previouslydamaged{ nullptr };
	f32 damage;
public:
	LaserEntity(AEVec2 pos, GameObjectEntity* _owner, f32 _damage);
	virtual ~LaserEntity();
	virtual void PostUpdate(const f32& dt);
	virtual void OnCollide(GameObjectEntity* other);
};

#endif