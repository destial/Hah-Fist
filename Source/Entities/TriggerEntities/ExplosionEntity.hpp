#pragma once
#ifndef __EXPLOSION_ENTITY_H
#define __EXPLOSION_ENTITY_H
#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"

class ExplosionEntity : public GameObjectEntity {
protected:
	GameObjectEntity* owner;
	GameObjectEntity* previouslydamaged{nullptr};
	f32 damage;
public:
	ExplosionEntity(AEVec2 pos, GameObjectEntity* owner, f32 damage);
	virtual ~ExplosionEntity();
	virtual void PostUpdate(const f32& dt);
	virtual void OnCollide(GameObjectEntity* other);
};

#endif