#pragma once
#ifndef __PHYSICSBODY_H
#define __PHYSICSBODY_H
#include "AEVec2.h"
class PhysicsBody {
public:
	enum STATE {
		IN_AIR = 0,
		ON_GROUND
	}state;
	AEVec2 gravity;
	AEVec2 force;
	f32 mass;
	f32 gravityScale;

	PhysicsBody(f32 mass = 1.f, f32 gravityScale = 1.f, AEVec2 gravity = AEVec2{0.0f, -98.f});
	~PhysicsBody();

	void ApplyGravity(AEVec2& velocity,const f32& dt);
};

#endif