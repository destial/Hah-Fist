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