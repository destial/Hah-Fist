#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"

AEVec2 World_To_Screen(float x, float y);
AEVec2 Screen_To_World(float x, float y);
AEVec2 Game_To_Screen(float x, float y);

#endif