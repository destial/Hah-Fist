#pragma once
#ifndef __AEOVERLOAD_H
#define __AEOVERLOAD_H
#include "AEMath.h"

static AEVec2 operator+ (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

static AEVec2 operator- (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

static AEVec2& operator+= (AEVec2& lhs, const AEVec2& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

static AEVec2& operator-= (AEVec2& lhs, const AEVec2& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

#endif