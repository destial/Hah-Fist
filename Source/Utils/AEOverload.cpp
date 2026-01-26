#include "AEOverload.hpp"
#include <cmath>

void AEVec2Rotate(AEVec2* pResult, AEVec2* pVec, float radians) {
	pResult->x = (pVec->x * AECos(radians)) - (pVec->y * AESin(radians));
	pResult->y = (pVec->x * AESin(radians)) + (pVec->y * AECos(radians));
}

f32 AEVec2Angle(AEVec2* vec1, AEVec2* vec2) {
	f32 dot = AEVec2DotProduct(vec1, vec2);
	return AEACos(dot / (AEVec2Length(vec1) * AEVec2Length(vec2)));
}

f32 AEVec2AngleCCW(AEVec2* vec1, AEVec2* vec2) {
	f32 u = (vec1->x * vec2->y) - (vec1->y * vec2->x);
	f32 v = (vec1->x * vec2->x) + (vec1->y * vec2->y);
	return std::atan2f(u, v);
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
	os << "AEVec2={" << rhs.x << ", " << rhs.y << '}';
	return os;
}

std::istream& operator>>(std::istream& is, AEVec2& rhs) {
	is >> rhs.x >> rhs.y;
	return is;
}

AEMtx33 operator* (const AEMtx33& lhs, const AEMtx33& rhs) {
	AEMtx33 result;
	AEMtx33Concat(&result, &lhs, &rhs);
	return result;
}

AEMtx33& operator*= (AEMtx33& lhs, const AEMtx33& rhs) {
	AEMtx33Concat(&lhs, &lhs, &rhs);
	return lhs;
}