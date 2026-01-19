#include "PhysicsBody.hpp"
#include "AEOverload.hpp"
PhysicsBody::PhysicsBody() : gravity{ 0.0f, -98.f }, force{}, mass{1.0f}, gravityScale{ 1.f }, state{IN_AIR}
{
}

PhysicsBody::PhysicsBody(AEVec2 gravity, AEVec2 force, f32 mass, f32 gravityScale)
{
	this->gravity = gravity;
	this->force = force;
	this->mass = mass;
	this->gravityScale = gravityScale;
	state = IN_AIR;
}

PhysicsBody::~PhysicsBody()
{
	// Empty
}

void PhysicsBody::ApplyGravity(AEVec2& velocity, const f32& dt)
{
	velocity += gravity * gravityScale * dt;// *dt;
}
