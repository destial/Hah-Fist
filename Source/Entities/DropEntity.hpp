#pragma once
#ifndef __DROP_ENTITY_H
#define __DROP_ENTITY_H
#include "GameObjectEntity.hpp"
#include "../Items/SpriteSheet.hpp"
class DropEntity : public GameObjectEntity {
protected:
	
public:
	DropEntity(AEVec2 pos = { 0.f,0.f }, f32 mass = 1.f, CollisionShape shape = CollisionShape::AABB);
	~DropEntity();

	void Update(const f32& dt) override;

	void OnCollide(GameObjectEntity* go) override;
	virtual void OnPickup(GameObjectEntity* pickupper);
};
#endif