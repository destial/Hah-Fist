#include "PhysicsBody.hpp"
#include "AEOverload.hpp"
PhysicsBody::PhysicsBody(f32 mass, f32 gravityScale, AEVec2 gravity)
{
	this->mass = mass;
	this->gravityScale = gravityScale;
	this->gravity = gravity;
	state = STATE::IN_AIR;
	vertical_state = VERTICAL_STATE::VS_FALLING;
	force = AEVec2{};
}

PhysicsBody::~PhysicsBody()
{
	// Empty
}

void PhysicsBody::UpdateStates(AEVec2& velocity, AEVec2& position, AEVec2& scale)
{
	if (state == IN_AIR)
	{
		if (velocity.y > 0)
		{
			vertical_state = VS_RISING;
		}
		else if (velocity.y < 0)
		{
			vertical_state = VS_FALLING;
		}
		else if (velocity.y == 0 && vertical_state == VS_FALLING)
		{
			vertical_state = VS_ON_GROUND;
			state = ON_GROUND;
		}
	}
	if (state == ON_GROUND)
	{
		if (velocity.y > 0)
		{
			state = IN_AIR;
		}
		else if (!is_colliding && position.y > scale.y * 0.5f)
		{
			state = IN_AIR;
		}
	}
}

void PhysicsBody::ApplyGravity(AEVec2& velocity, const f32& dt)
{
	if (state == IN_AIR)
	{
		velocity += gravity * gravityScale * dt;// *dt;
	}
}
