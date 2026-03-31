/*!
* @file PhysicsBody.hpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 19 January 2026
* @course CSD1451
* @brief This source file contains the declaration of a PhysicsBody class which is incharge
*		 of physics of the gameobjects, jumping & gravity.
*/
#pragma once
#ifndef __PHYSICSBODY_H
#define __PHYSICSBODY_H
#include "AEVec2.h"
#include <iostream>
/*!
* @brief PhysicsBody class that handles all things related to the physics of the gameobject.
*		 It is the composition of every gameobject.
*/
class PhysicsBody {
public:
	enum struct STATE {
		IN_AIR = 0,
		ON_GROUND
	} state;

	enum struct VERTICAL_STATE {
		VS_RISING = 0,
		VS_FALLING,
		VS_ON_GROUND,
	} vertical_state;

	AEVec2 gravity;
	AEVec2 force;
	f32 mass;
	f32 gravityScale;
	f32 air_strength{ 0.0f };

	bool is_standing_above = false;
	/*!
	* @brief Default Constructor which initialises the mass, gravityScale, gravity based on what the user has input
	* @param mass - Mass of the gameobject
	* @param gravityScale - Gravity's scale, higher = more gravity being applied.
	* @param gravity - Gravity's direction
	*/
	PhysicsBody(f32 mass = 1.f, f32 gravityScale = 1.f, AEVec2 gravity = AEVec2{0.0f, -98.f});

	/*!
	* @brief Destructor of the PhysicsBody class which uses the synthesised destructor compiler gives.
	*/
	~PhysicsBody() = default;

	/*!
	* @brief UpdateStates function which updates the states of the gameobject based on its position
	*	     and velocity accordingly.
	* @param velocity - Velocity of the gameobject
	* @param position - Position of the gameobject
	* @param scale - Scale of the gameobject
	* @return void
	*/
	void UpdateStates(AEVec2& velocity, AEVec2& position, AEVec2& scale);

	/*!
	* @brief ApplyGravity function which applies the gravity to the gameobject.
	* @param velocity - Velocity of the gameobject
	* @param dt - Delta time - Time between every frame
	* @return void
	*/
	void ApplyGravity(AEVec2& velocity,const f32& dt);
};

/*!
* @brief operator overload of std::ostream operator<<. Outputs the current state of the PhysicsBody to
*		 the output stream.
* @param std::ostream& - Reference to the output stream
* @param const PhysicsBody::STATE& - const reference to the current state of the PhysicsBody
* @return std::ostream&
*/
std::ostream& operator<<(std::ostream&, const PhysicsBody::STATE&);

/*!
* @brief operator overload of std::ostream operator<<. Outputs the current vertical state of the PhysicsBody to
*		 the output stream.
* @param std::ostream& - Reference to the output stream
* @param const PhysicsBody::VERTICAL_STATE& - const reference to the current vertical state of the PhysicsBody
* @return std::ostream&
*/
std::ostream& operator<<(std::ostream&, const PhysicsBody::VERTICAL_STATE&);
#endif