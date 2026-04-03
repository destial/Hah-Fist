/*!
* @file WeaponEntity.hpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 20th January 2026
* @course CSD1451
* @brief This source file declares the weapon class and the member functions.
*		 The weapon class object handles basic functionality and variables required
*		 by all inherited weapons.
*/
#pragma once
#ifndef __WEAPONENTITY_H
#define __WEAPONENTITY_H
#include "GameObjectEntity.hpp"

/*!
* @brief WeaponEntity class manages basic functionality of all weapons.
*/
class WeaponEntity : public GameObjectEntity {
protected:
	GameObjectEntity* player_entity{ nullptr }; // pointer to the player_entity which owns this weapon.
	bool weapon_channels{ false }; // Does this weapon channel or instant use on click?
	bool channelling{ false }; // Is this weapon currently channelling?
	f32 max_channel_time{ 2.0f }; //Maximum duration required to channel this weapon for full effect.
	f32 channel_timer{ 0.0f }; //Current elapsed time of channelling.
	AEVec2 weapon_direction{ 0.f, 0.f }; //What direction was this weapon facing during the last attack?
	float cd_timer{ 0.0f }; // Current elapsed time of the weapon's cooldown.
	float cd_duration{2.0f}; // Maximum cooldown time of the weapon.
	float damage{ 5.f }; // Damage this weapon does.
public:
	/*!
	* @brief Constructor function for a weapon. Initializes
	* basic information for the weapon.
	* @param AEVec2 pos - Position to spawn the weapon.
	*/
	WeaponEntity(AEVec2 pos = { 0.f, 0.f });

	/*!
	* @brief Destructor function for a weapon.
	*/
	virtual ~WeaponEntity();

	/*!
	* @brief PreUpdate override for the weapon.
	* Handles the positioning and rotation of a weapon.
	* @param const f32& dt - delta time.
	*/
	virtual void PreUpdate(const f32& dt);

	/*!
	* @brief Update override for the weapon.
	* Handles the input detection for the weapon and blocking input conditionally.
	* @param const f32& dt - delta time.
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Render override for the weapon.
	* Conditionally renders if the player entity still exists.
	* Also defines rendering for debugging.
	*/
	virtual void Render();

	/*!
	* @brief Member function, overridden in inherited classes
	* to define behaviour when an attack happens.
	*/
	virtual void Attack() = 0;

	/*!
	* @brief Member function to reset data of a weapon.
	*/
	virtual void ResetWeapon();

	/*!
	* @brief Helper function to get the current pointed direction from the cursor.
	* @return AEVec2 vector of the pointed direction.
	*/
	AEVec2 GetAttackDirection() const;

	/*!
	* @brief Helper function to get the current value of the channel timer.
	* @return f32 value of channel_timer
	*/
	f32 GetChannelTimer() const;

	/*!
	* @brief Helper function to get the maximum channel time.
	* @return f32 value of max_channel_time
	*/
	f32 GetMaxChannelTime() const;

	/*!
	* @brief Helper function to get the current elapsed time of the weapon's cooldown.
	* @return f32 value of cd_timer
	*/
	f32 GetCooldownTimer() const;
	/*!
	* @brief Helper function to get the maximum duration of the weapon's cooldown.
	* @return f32 value of cd_duration
	*/
	f32 GetCooldownDuration() const;
};

#endif