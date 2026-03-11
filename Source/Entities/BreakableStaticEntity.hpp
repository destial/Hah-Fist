#pragma once
#ifndef __BREAKABLE_STATIC_ENTITY_H
#define __BREAKABLE_STATIC_ENTITY_H
#include "StaticEntity.hpp"

class BreakableStaticEntity : StaticEntity {
public:
	BreakableStaticEntity(STATIC_TYPE static_type, AEVec2 pos = { 0.f,0.f }, f32 mass = 1.f, CollisionShape shape = CollisionShape::AABB, PhysicsType go_type = PhysicsType::STATIC);
	~BreakableStaticEntity();

	void Update(const f32& dt);
	void OnBroken();
};
#endif