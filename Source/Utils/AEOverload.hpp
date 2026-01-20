#pragma once
#ifndef __AEOVERLOAD_H
#define __AEOVERLOAD_H
#include "AEMath.h"
#include <iostream>

AEVec2 operator+ (const AEVec2& lhs, const AEVec2& rhs);

AEVec2 operator- (const AEVec2& lhs, const AEVec2& rhs);

AEVec2 operator* (const AEVec2& lhs, f32 magnitude);

AEVec2 operator* (const float& scalar, const AEVec2& rhs);

AEVec2 operator/ (const AEVec2& lhs, f32 magnitude);

AEVec2& operator+= (AEVec2& lhs, const AEVec2& rhs);

AEVec2& operator-= (AEVec2& lhs, const AEVec2& rhs);

AEVec2& operator*= (AEVec2& lhs, f32 magnitude);

AEVec2& operator/= (AEVec2& lhs, f32 magnitude);

std::ostream& operator<< (std::ostream& os, const AEVec2& rhs);

#endif