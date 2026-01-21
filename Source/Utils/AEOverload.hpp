#pragma once
#ifndef __AEOVERLOAD_H
#define __AEOVERLOAD_H
#include "AEMath.h"
#include <iostream>

/*
* @brief Addition operator between two vectors
* @return A new vector
*/
AEVec2 operator+ (const AEVec2& lhs, const AEVec2& rhs);

/*
* @brief Subtraction operator between two vectors
* @return A new vector
*/
AEVec2 operator- (const AEVec2& lhs, const AEVec2& rhs);

/*
* @brief Scalar operator between a vector and magnitude
* @return A new vector
*/
AEVec2 operator* (const AEVec2& lhs, f32 magnitude);

/*
* @brief Dot product operator between two vectors
* @return The dot product
*/
f32 operator* (const AEVec2& lhs, const AEVec2& rhs);

/*
* @brief Division operator between a vector and magnitude
* @return A new vector
*/
AEVec2 operator/ (const AEVec2& lhs, f32 magnitude);

/*
* @brief Addition operator between two vectors
* @return A reference to the lhs vector
*/
AEVec2& operator+= (AEVec2& lhs, const AEVec2& rhs);

/*
* @brief Subtraction operator between two vectors
* @return A reference to the lhs vector
*/
AEVec2& operator-= (AEVec2& lhs, const AEVec2& rhs);

/*
* @brief Scalar operator between a vector and magnitude
* @return A reference to the lhs vector
*/
AEVec2& operator*= (AEVec2& lhs, f32 magnitude);

/*
* @brief Division operator between a vector and magnitude
* @return A reference to the lhs vector
*/
AEVec2& operator/= (AEVec2& lhs, f32 magnitude);

/*
* @brief Output stream operator for a vector
* @return Output formatted as {x,y}
*/
std::ostream& operator<< (std::ostream& os, const AEVec2& rhs);

/*
* @brief Input stream operator for a vector
* @return Input formatted from x y
*/
std::istream& operator>> (std::istream& is, AEVec2& rhs);

#endif