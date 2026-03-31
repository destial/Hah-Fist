/*!
* @file FingerGunEntity.hpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @date 8th March 2026
* @course CSD1451
* @brief This source file declares the FingerGunWeapon class which inherits from the weapon class.
* This weapons fires a projectile when the player clicks.
*/

#pragma once
#ifndef __FINGERGUNENTITY_H
#define __FINGERGUNENTITY_H
#include "../WeaponEntity.hpp"

/*!
* @brief FingerGunWeapon class manages custom behaviour of the FingerGunWeapon
*/
class FingerGunWeapon : public WeaponEntity {
protected:

public:
	/*!
	* @brief Constructor function for a FingerGunWeapon. Initializes the sprite and configuration for a
	* FingerGunWeapon.
	* @param AEVec2 pos - Position to spawn the weapon.
	*/
	FingerGunWeapon(AEVec2 pos);

	/*!
	* @brief Destructor function for a FingerGunWeapon.
	*/
	virtual ~FingerGunWeapon();

	/*!
	* @brief Update override for the FingerGunWeapon.
	* Blocks updates if this is not active.
	* @param const f32& dt - delta time.
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief OnCollide override for the FingerGunWeapon.
	* Does nothing.
	*/
	virtual void OnCollide(GameObjectEntity* go);

	/*!
	* @brief Attack override for the FingerGunWeapon.
	* Spawns a projectile and initializes it using the information
	* from this class.
	*/
	virtual void Attack();
};
#endif
