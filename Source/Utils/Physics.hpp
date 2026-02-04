#pragma once
#ifndef __PHYSICS_H
#define __PHYSICS_H
#include "AEEngine.h"

namespace Physics
{
	struct AABB
	{
		AEVec2 min;
		AEVec2 max;

		bool CanFit(AABB& a);
		bool Intersects(AABB& a);
	};


}

#endif