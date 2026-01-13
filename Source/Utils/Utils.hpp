#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"

AEVec2 World_To_Screen(float x, float y, float world_width, float world_height);

AEVec2 Screen_To_World(float x, float y, float world_width, float world_height);

#endif