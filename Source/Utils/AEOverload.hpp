#pragma once
#ifndef __AEOVERLOAD_H
#define __AEOVERLOAD_H
#include "AEMath.h"
#include <iostream>

static AEVec2 operator+ (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

static AEVec2 operator- (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

static AEVec2 operator* (const AEVec2& lhs, f32 magnitude) {
	return { lhs.x * magnitude, lhs.y * magnitude };
}

static AEVec2 operator/ (const AEVec2& lhs, f32 magnitude) {
	return { lhs.x / magnitude, lhs.y / magnitude };
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

static AEVec2& operator*= (AEVec2& lhs, f32 magnitude) {
	lhs.x *= magnitude;
	lhs.y *= magnitude;
	return lhs;
}

static AEVec2& operator/= (AEVec2& lhs, f32 magnitude) {
	lhs.x /= magnitude;
	lhs.y /= magnitude;
	return lhs;
}

static std::ostream& operator<< (std::ostream& os, const AEVec2& rhs) {
	os << '{' << rhs.x << ", " << rhs.y << '}';
	return os;
}

#endif