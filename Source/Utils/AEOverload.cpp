/*!
* @file AEOverload.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 7 January 2026
* @course CSD1451
* @brief Declaration of overload operators for the AEVec2 and AEMtx33 classes
*/
#include "AEOverload.hpp"
#include <cmath>

/*
* @brief Additional rotation Vector utility function
* @param result - The destination to store the results after rotation
* @param pVec - The vector to rotate
* @param radians - The rotation in radians
* @return The rotated vector
*/
void AEVec2Rotate(AEVec2* pResult, AEVec2* pVec, float radians) {
	pResult->x = (pVec->x * AECos(-radians)) - (pVec->y * AESin(-radians));
	pResult->y = (pVec->x * AESin(-radians)) + (pVec->y * AECos(-radians));
}

/*
* @brief Additional angle Vector utility function
* @param vec1 - The first vector
* @param vec2 - The second vector
* @return The minimum angle between two vectors
*/
f32 AEVec2Angle(AEVec2* vec1, AEVec2* vec2) {
	f32 dot = AEVec2DotProduct(vec1, vec2);
	return AEACos(dot / (AEVec2Length(vec1) * AEVec2Length(vec2)));
}

/*
* @brief Additional angle Vector utility function
* @param vec1 - The first vector
* @param vec2 - The second vector
* @return The counter-clockwise angle between two vectors
*/
f32 AEVec2AngleCCW(AEVec2* vec1, AEVec2* vec2) {
	f32 u = (vec1->x * vec2->y) - (vec1->y * vec2->x);
	f32 v = (vec1->x * vec2->x) + (vec1->y * vec2->y);
	return std::atan2f(u, v);
}

/*
* @brief Addition operator between two vectors
* @return A new vector
*/
AEVec2 operator+ (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

/*
* @brief Subtraction operator between two vectors
* @return A new vector
*/
AEVec2 operator- (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

/*
* @brief Scalar operator between a vector and magnitude
* @return A new vector
*/
AEVec2 operator* (const AEVec2& lhs, f32 magnitude) {
	return { lhs.x * magnitude, lhs.y * magnitude };
}

/*
* @brief Scalar operator between a vector and magnitude
* @return A new vector
*/
AEVec2 operator* (f32 magnitude, const AEVec2& rhs) {
	return { rhs.x * magnitude, rhs.y * magnitude };
}

/*
* @brief Dot product operator between two vectors
* @return The dot product
*/
f32 operator* (const AEVec2& lhs, const AEVec2& rhs) {
	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

/*
* @brief Division operator between a vector and magnitude
* @return A new vector
*/
AEVec2 operator/ (const AEVec2& lhs, f32 magnitude) {
	AEVec2 o{ lhs };
	o /= magnitude;
	return o;
}

/*
* @brief Division operator between two vectors
* @return A new vector
*/
AEVec2 operator/ (const AEVec2& lhs, const AEVec2& rhs) {
	return { lhs.x / rhs.x, lhs.y / rhs.y };
}

/*
* @brief Addition operator between two vectors
* @return A reference to the lhs vector
*/
AEVec2& operator+= (AEVec2& lhs, const AEVec2& rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

/*
* @brief Subtraction operator between two vectors
* @return A reference to the lhs vector
*/
AEVec2& operator-= (AEVec2& lhs, const AEVec2& rhs) {
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

/*
* @brief Scalar operator between a vector and magnitude
* @return A reference to the lhs vector
*/
AEVec2& operator*= (AEVec2& lhs, f32 magnitude) {
	lhs.x *= magnitude;
	lhs.y *= magnitude;
	return lhs;
}

/*
* @brief Division operator between a vector and magnitude
* @return A reference to the lhs vector
*/
AEVec2& operator/= (AEVec2& lhs, f32 magnitude) {
	if (magnitude == 0) {
		throw std::exception("Division by zero!");
	}
	lhs.x /= magnitude;
	lhs.y /= magnitude;
	return lhs;
}

/*
* @brief Checks if a vector matches the scalar
* @return A boolean - True or false
*/
bool operator== (const AEVec2& lhs, const float& scalar) {
	return (lhs.x == scalar && lhs.y == scalar);
}

/*
* @brief Output stream operator for a vector
* @return Output formatted as {x,y}
*/
std::ostream& operator<< (std::ostream& os, const AEVec2& rhs) {
	os << '{' << rhs.x << ", " << rhs.y << '}';
	return os;
}

/*
* @brief Input stream operator for a vector
* @return Input formatted from x y
*/
std::istream& operator>> (std::istream& is, AEVec2& rhs) {
	char c;
	return is >> c >> rhs.x >> c >> rhs.y >> c;
}

/*
* @brief Multiplication operator between two matrices
* @return A new matrix
*/
AEMtx33 operator* (const AEMtx33& lhs, const AEMtx33& rhs) {
	AEMtx33 result;
	AEMtx33Concat(&result, &lhs, &rhs);
	return result;
}

/*
* @brief Multiplication operator between two matrices
* @return A reference to the lhs matrix
*/
AEMtx33& operator*= (AEMtx33& lhs, const AEMtx33& rhs) {
	AEMtx33Concat(&lhs, &lhs, &rhs);
	return lhs;
}