#pragma once
#ifndef __GAMEOBJECTENTITY_H
#define __GAMEOBJECTENTITY_H
#include "BaseEntity.hpp"
#include "../Utils/PhysicsBody.hpp"

class GameObjectEntity : public BaseEntity {
public:
	PhysicsBody* pBody;
	enum struct CollisionShape {
		CIRCLE, AABB, TOTAL
	} shape;
	enum struct PhysicsType {
		STATIC, DYNAMIC, TRIGGER, TOTAL
	} go_type;
	AEVec2 prev_position;
	f32 frictionMultiplier{ 1.0 };
	GameObjectEntity();
	GameObjectEntity(AEVec2 pos = { 0.f, 0.f }, f32 mass = 1.0f, CollisionShape type = CollisionShape::AABB, PhysicsType go_type = PhysicsType::DYNAMIC);
	virtual ~GameObjectEntity();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void OnCollide(GameObjectEntity* go);

	bool isActive;
	f32 health;
	f32 max_health;
	f32 damage;
};
#endif