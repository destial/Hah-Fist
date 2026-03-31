/*!
* @file TurboFistEntity.hpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @date 24th February 2026
* @course CSD1451
* @brief This source file declares the TurboFistWeapon which inherits the weapon class.
* This weapon channels and releases to launch the player forward, speed of travel
* determined by the duration of travel.
*/

#pragma once
#ifndef __TURBOFISTENTITY_H
#define __TURBOFISTENTITY_H
#include "../WeaponEntity.hpp"

/*!
* @brief TurboFistWeapon class manages custom behaviour of the TurboFistWeapon
*/
class TurboFistWeapon : public WeaponEntity {
protected:
	f32 max_dash_time = 0.25f; //The maximum duration this weapon dashes for.
	f32 dash_timer = 0.0f; //Current elapsed duration of the dash.

	bool dashing = false; //Is this weapon currently dashing?
	f32 player_original_mass = 0.0f; //Original mass of the player.
public:
	/*!
	* @brief Constructor function for a TurboFistWeapon. Initializes the sprite and configuration for a 
	* TurboFistWeapon.
	* @param AEVec2 pos - Position to spawn the weapon.
	* @param f32 og_mass - Original mass of the player.
	*/
	TurboFistWeapon(AEVec2 pos, f32 og_mass);

	/*!
	* @brief Destructor function for a TurboFistWeapon.
	*/
	virtual ~TurboFistWeapon();

	/*!
	* @brief Update override for the TurboFistWeapon.
	* Handles the dash timer and resetting the mass of the
	* player after the dash completes.
	* @param const f32& dt - delta time.
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Render override for the TurboFistWeapon.
	* Adds debugging rendering for the TurboFistWeapon.
	*/
	virtual void Render();

	/*!
	* @brief OnCollide override for the TurboFistWeapon.
	* Defines custom behaviour for the interactions depending
	* on what this object collides with.
	* @param GameObjectEntity* go - object this collided with.
	*/
	virtual void OnCollide(GameObjectEntity* go);

	/*!
	* @brief Attack override for the TurboFistWeapon.
	* Initializes the values required for the TurboFistWeapon's
	* attack; Dash timer, invulnerability duration, increasing the player's mass, reducing the player's friction.
	*/
	virtual void Attack();

	/*!
	* @brief Helper function to get the attack strength multiplier based on the charge duration.
	* @return f32 value of the current attack strength.
	*/
	f32 GetCurrentAttackStrength() const;

	/*!
	* @brief ResetWeapon override.
	* Resets dash_timer and dashing to false.
	*/
	virtual void ResetWeapon();
};

#endif