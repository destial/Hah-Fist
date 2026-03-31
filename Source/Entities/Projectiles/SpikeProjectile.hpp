/*!
* @file SpikeProjectile.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 13 March 2026
* @course CSD1451
* @brief Declaration of the SpikeProjectile class which represents a projectile
*        with modified movement behavior. This projectile may start slow and
*        accelerate over time, extending BaseProjectile functionality.
*/
#pragma once
#ifndef __SPIKEPROJECTILE_HPP
#define __SPIKEPROJECTILE_HPP
#include "BaseProjectile.hpp"
/*!
* @brief SpikeProjectile class that handles a projectile with delayed or gradual
*        acceleration behavior. Extends BaseProjectile to modify movement dynamics.
*/
class SpikeProjectile : public BaseProjectile {
private:
public:
	/*!
	* @brief Constructs a SpikeProjectile with position, direction, speed, damage, and owner
	* @param pos - Initial position of the projectile
	* @param dir - Direction vector of movement
	* @param speed - Maximum movement speed of the projectile
	* @param dmg - Damage dealt upon collision
	* @param own - Pointer to the owning entity
	* @return None
	*/
	SpikeProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own);
	/*!
	* @brief Destructor for SpikeProjectile
	* @return None
	*/
	virtual ~SpikeProjectile();
};

#endif