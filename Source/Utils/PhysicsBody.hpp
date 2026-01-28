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

	enum VERTICAL_STATE {
		VS_RISING = 0,
		VS_FALLING,
		VS_ON_GROUND,
	}vertical_state;

	AEVec2 gravity;
	AEVec2 force;
	f32 mass;
	f32 gravityScale;

	bool is_colliding = false;

	PhysicsBody(f32 mass = 1.f, f32 gravityScale = 1.f, AEVec2 gravity = AEVec2{0.0f, -98.f});
	~PhysicsBody();

	void UpdateStates(AEVec2& velocity, AEVec2& position, AEVec2& scale);
	void ApplyGravity(AEVec2& velocity,const f32& dt);
};

#endif