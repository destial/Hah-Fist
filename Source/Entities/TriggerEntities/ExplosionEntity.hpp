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
	ExplosionEntity(AEVec2 pos, GameObjectEntity* _owner, f32 _damage);
	~ExplosionEntity();
	void Update(const f32& dt);
	void PostUpdate(const f32& dt);
	void OnCollide(GameObjectEntity* other);
};
#endif