/*!
* @file PhysicsBody.hpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 19 January 2026
* @course CSD1451
* @brief This source file contains the definitions of a PhysicsBody class which is incharge
*		 of physics of the game_objects, jumping & gravity.
*/
#include "PhysicsBody.hpp"
#include "AEOverload.hpp"
/*!
* @brief Default Constructor which initialises the mass, gravity_scale, gravity based on what the user has input
* @param mass - Mass of the gameobject
* @param gravity_scale - Gravity's scale, higher = more gravity being applied.
* @param gravity - Gravity's direction
*/
PhysicsBody::PhysicsBody(f32 mass, f32 gravity_scale, AEVec2 gravity)
{
	this->mass = mass;
	this->gravity_scale = gravity_scale;
	this->gravity = gravity;
	state = STATE::IN_AIR;
	vertical_state = VERTICAL_STATE::VS_FALLING;
	force = AEVec2{};
}

/*!
* @brief UpdateStates function which updates the states of the gameobject based on its position
*	     and velocity accordingly.
* @param velocity - Velocity of the gameobject
* @param position - Position of the gameobject
* @param scale - Scale of the gameobject
* @return void
*/
void PhysicsBody::UpdateStates(AEVec2& velocity, AEVec2& position, AEVec2& scale)
{
	if (state == STATE::IN_AIR)
	{
		if (velocity.y > 0)
		{
			vertical_state = VERTICAL_STATE::VS_RISING;
		}
		else if (velocity.y < 0)
		{
			vertical_state = VERTICAL_STATE::VS_FALLING;
		}
		else if (velocity.y == 0 && vertical_state == VERTICAL_STATE::VS_FALLING)
		{
			vertical_state = VERTICAL_STATE::VS_ON_GROUND;
			state = STATE::ON_GROUND;
		}
	}
	if (state == STATE::ON_GROUND)
	{
		if (velocity.y > 0)
		{
			state = STATE::IN_AIR;
			air_strength = 1.0;
		}
		else if (!is_standing_above && position.y > scale.y * 0.5f)
		{
			state = STATE::IN_AIR;
			air_strength = 1.0;
		}
	}
}

/*!
* @brief ApplyGravity function which applies the gravity to the gameobject.
* @param velocity - Velocity of the gameobject
* @param dt - Delta time - Time between every frame
* @return void
*/
void PhysicsBody::ApplyGravity(AEVec2& velocity, const f32& dt)
{
	if (state == STATE::IN_AIR)
	{
		velocity += gravity * gravity_scale * dt;// *dt;
	}
}

// Debug tool for checking the player's state
/*!
* @brief operator overload of std::ostream operator<<. Outputs the current state of the PhysicsBody to
*		 the output stream.
* @param std::ostream& - Reference to the output stream
* @param const PhysicsBody::STATE& - const reference to the current state of the PhysicsBody
* @return std::ostream&
*/
std::ostream& operator<<(std::ostream& os, const PhysicsBody::STATE& pState) {
	os << (pState == PhysicsBody::STATE::IN_AIR ? "IN_AIR" : pState == PhysicsBody::STATE::ON_GROUND ? "ON_GROUND" : "FALLING");
	return os;
}

/*!
* @brief operator overload of std::ostream operator<<. Outputs the current vertical state of the PhysicsBody to
*		 the output stream.
* @param std::ostream& - Reference to the output stream
* @param const PhysicsBody::VERTICAL_STATE& - const reference to the current vertical state of the PhysicsBody
* @return std::ostream&
*/
std::ostream& operator<<(std::ostream& os, const PhysicsBody::VERTICAL_STATE& vState) {
	os << (vState == PhysicsBody::VERTICAL_STATE::VS_FALLING ? "VS_FALLING" : vState == PhysicsBody::VERTICAL_STATE::VS_ON_GROUND ? "ON_GROUND" : "VS_RISING");
	return os;
}