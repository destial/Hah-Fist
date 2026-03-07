#pragma once
#ifndef __STATIC_ENTITY_H
#define __STATIC_ENTITY_H
#include "GameObjectEntity.hpp"
class StaticEntity : public GameObjectEntity {
public:
	enum class STATIC_TYPE {
		TYPE_WALL,
		TYPE_PLATFORM
	}static_type;
public:
	StaticEntity(STATIC_TYPE static_type,AEVec2 pos = { 0.f,0.f }, f32 mass = 1.f, CollisionShape shape = CollisionShape::AABB, PhysicsType go_type = PhysicsType::STATIC);
	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void OnCollide(GameObjectEntity* go) override;

	STATIC_TYPE GetStaticType() const;

};
#endif