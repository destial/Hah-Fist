#include "AEOverload.hpp"

void AEVec2Rotate(AEVec2* pResult, AEVec2* pVec, float radians) {
	pResult->x = (pVec->x * AECos(radians)) - (pVec->y * AESin(radians));
	pResult->y = (pVec->x * AESin(radians)) + (pVec->y * AECos(radians));
}

AEVec2 operator+ (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

AEVec2 operator- (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

AEVec2 operator* (const AEVec2& lhs, f32 magnitude) {
	return { lhs.x * magnitude, lhs.y * magnitude };
}

f32 operator* (const AEVec2& lhs, const AEVec2& rhs) {
	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
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

std::istream& operator>>(std::istream& is, AEVec2& rhs) {
	is >> rhs.x >> rhs.y;
	return is;
}
