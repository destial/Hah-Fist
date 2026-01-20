#include "PhysicsBody.hpp"
#include "AEOverload.hpp"
PhysicsBody::PhysicsBody(f32 mass, f32 gravityScale, AEVec2 gravity)
{
	this->mass = mass;
	this->gravityScale = gravityScale;
	this->gravity = gravity;
	state = STATE::IN_AIR;
	force = AEVec2{};
}

PhysicsBody::~PhysicsBody()
{
	// Empty
}

void PhysicsBody::ApplyGravity(AEVec2& velocity, const f32& dt)
{
	velocity += gravity * gravityScale * dt;// *dt;
}
