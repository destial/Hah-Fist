#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"

struct Color {
	int a, r, g, b;
};

AEVec2 World_To_Screen(float x, float y);
AEVec2 Screen_To_World(float x, float y);
AEVec2 Game_To_Screen(float x, float y);
AEVec2 Scale_To_Screen(float x, float y);

u32 ConvertToColor(int red, int green, int blue, int alpha = 255);
Color ConvertFromColor(u32 color);

float GetWorldWidth(void);
float GetWorldHeight(void);

#endif