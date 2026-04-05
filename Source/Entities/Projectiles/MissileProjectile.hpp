/*!
* @file MissileProjectile.hpp
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 12th March 2026
* @course CSD1451
* @brief This source file declares the MissileProjectile class and its member functions, which are
* Destructable objects in the game.
*/
#pragma once
#ifndef __MISSILEPROJECTILE_HPP
#define __MISSILEPROJECTILE_HPP
#include "BaseProjectile.hpp"

class MissileProjectile : public BaseProjectile {
private:
	f32 slow_start_time{ 1.0f };

public:
	MissileProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own);
	virtual ~MissileProjectile();
};

#endif