/*!
* @file ExplosiveProjectile.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 13 March 2026
* @course CSD1451
* @brief ExplosiveProjectile class that represents a projectile which triggers
*        an explosion upon impact or expiration. Extends BaseProjectile to add
*        area-of-effect damage behavior.
*/
#pragma once
#ifndef __EXPLOSIVEPROJECTILE_HPP
#define __EXPLOSIVEPROJECTILE_HPP
#include "BaseProjectile.hpp"
/*!
* @brief ExplosiveProjectile class that represents a projectile which triggers
*        an explosion upon impact or expiration. Extends BaseProjectile to add
*        area-of-effect damage behavior.
*/
class ExplosiveProjectile : public BaseProjectile {
public:
	/*!
	* @brief Constructs an ExplosiveProjectile with optional lerp behavior
	* @param pos - Initial position of the projectile
	* @param dir - Direction vector of movement
	* @param speed - Movement speed of the projectile
	* @param dmg - Damage dealt by the explosion
	* @param own - Pointer to the owning entity
	* @param lerp - Whether the projectile uses interpolated movement (optional)
	* @return None
	*/
	ExplosiveProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own, bool lerp = false);
	/*!
	* @brief Destructor for ExplosiveProjectile
	* @return None
	*/
	virtual ~ExplosiveProjectile();
	/*!
	* @brief Handles collision by triggering explosion effects instead of standard hit logic
	* @param other - Pointer to the object collided with
	* @return None
	*/
	virtual void OnHit(GameObjectEntity* other);
	/*!
	* @brief Handles projectile expiration by triggering an explosion
	* @return None
	*/
	void OnExpire() override;
};

#endif