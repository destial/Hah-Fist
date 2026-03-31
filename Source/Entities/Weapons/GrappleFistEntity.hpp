/*!
* @file GrappleFistWeapon.hpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @date 8th March 2026
* @course CSD1451
* @brief This source file declares the GrappleFistWeapon class which inherits from the weapon class.
* This weapon grapples the player towards structures and pulls entities towards the player.
*/
#pragma once
#ifndef __GRAPPLEFISTENTITY_H
#define __GRAPPLEFISTENTITY_H
#include "../WeaponEntity.hpp"

/*!
* @brief GrappleFistWeapon class manages custom behaviour.
*/
class GrappleFistWeapon : public WeaponEntity {
protected:
	f32 grappleSpeedMult{ 50.0f }; //Multiplier for travel speed of the grapple.
	enum GRAPPLE_STATE {
		INACTIVE,
		SHOOTING,
		RETURNING,
		HOOKING_OBJECT_TO_PLAYER,
		HOOKING_PLAYER_TO_STATIC_OBJECT,
		HOOKING_PLAYER_TO_DYNAMIC_OBJECT,
	} grappleState{ INACTIVE }; // Current state of this weapon's grapple.
	f32 travelDuration{ 0.0f }; // How long this weapon has been travelling.
	GameObjectEntity* grappledObject{ nullptr }; // Pointer to the current grappled object.
	AEVec2 snapshottedAttackDirection; // The travel direction which is snapshotted when the attack is called.
public:
	/*!
	* @brief Constructor function for a GrappleFistWeapon. Initializes the sprite and configuration for a
	* GrappleFistWeapon.
	* @param AEVec2 pos - Position to spawn the weapon.
	*/
	GrappleFistWeapon(AEVec2 pos);

	/*!
	* @brief Destructor function for a GrappleFistWeapon.
	*/
	virtual ~GrappleFistWeapon();

	/*!
	* @brief PreUpdate override for the GrappleFistWeapon.
	* Blocks the default behaviour of a weapon if the weapon is currently grappling.
	* @param const f32& dt - delta time.
	*/
	virtual void PreUpdate(const f32& dt);

	/*!
	* @brief Update override for the GrappleFistWeapon.
	* Blocks the base update if this object is inactive.
	* @param const f32& dt - delta time.
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief PostUpdate override for the GrappleFistWeapon.
	* Handles the behaviour of the grapple depending on its current state.
	* @param const f32& dt - delta time.
	*/
	virtual void PostUpdate(const f32& dt);

	/*!
	* @brief OnCollide override for the GrappleFistWeapon.
	* Defines what state the weapon should change to depending on the object
	* this collides with.
	* @param GameObjectEntity* go - object this collides with.
	*/
	virtual void OnCollide(GameObjectEntity* go);

	/*!
	* @brief Attack override for the GrappleFistWeapon.
	* Configures the state and snapshots the direction of firing.
	*/
	virtual void Attack();

	/*!
	* @brief ResetWeapon override for the GrappleFistWeapon.
	* Resets states and variables back to 0
	*/
	virtual void ResetWeapon();
};
#endif