/*!
* @file BreakableCrateEntity.cpp
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 12th March 2026
* @course CSD1451
* @brief This source file declares the BreakableCrateEntity class and its member functions, which are
* Destructable objects in the game that drop coins.
*/
#pragma once
#ifndef __BREAKABLE_CRATE_ENTITY_H
#define __BREAKABLE_CRATE_ENTITY_H
#include "BreakableStaticEntity.hpp"

class BreakableCrateEntity : public BreakableStaticEntity {
public:
	BreakableCrateEntity(AEVec2 pos);
	virtual ~BreakableCrateEntity();

	virtual void OnBroken();
};

#endif