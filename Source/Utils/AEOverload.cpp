#include "AEOverload.hpp"
#include <iostream>

AEVec2 operator+ (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

AEVec2 operator- (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

AEVec2 operator* (const AEVec2& lhs, f32 magnitude) {
	return { lhs.x * magnitude, lhs.y * magnitude };
}

AEVec2 operator/ (const AEVec2& lhs, f32 magnitude) {
	return { lhs.x / magnitude, lhs.y / magnitude };
}

AEVec2& operator+= (AEVec2& lhs, const AEVec2& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

AEVec2& operator-= (AEVec2& lhs, const AEVec2& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

AEVec2& operator*= (AEVec2& lhs, f32 magnitude) {
	lhs.x *= magnitude;
	lhs.y *= magnitude;
	return lhs;
}

AEVec2& operator/= (AEVec2& lhs, f32 magnitude) {
	lhs.x /= magnitude;
	lhs.y /= magnitude;
	return lhs;
}

std::ostream& operator<< (std::ostream& os, const AEVec2& rhs) {
	os << '{' << rhs.x << ", " << rhs.y << '}';
	return os;
}
