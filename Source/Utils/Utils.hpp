#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"

struct Color {
	union {
		unsigned char a;
		unsigned char alpha;
	};
	union {
		unsigned char r;
		unsigned char red;
	};
	union {
		unsigned char g;
		unsigned char green;
	};
	union {
		unsigned char b;
		unsigned char blue;
	};
};

namespace Utils {
	AEVec2 World_To_Screen(float x, float y);
	AEVec2 Screen_To_World(float x, float y);
	AEVec2 Game_To_Screen(float x, float y);
	AEVec2 Scale_To_Screen(float x, float y);
	AEVec2 Game_To_TextScreen(float x, float y);

	const AEVec2 Get_Mouse_World();

	u32 PackColor(int red, int green, int blue, int alpha = 255);
	u32 PackColor(Color& color);
	Color UnpackColor(u32 color);

	const float GetWorldWidth(void);
	const float GetWorldHeight(void);
	const float GetDeltaTime(void);

	bool CircleCircleCollision(AEVec2& center_pos_1, const f32& radius_1, AEVec2& center_pos_2, const f32& radius_2);
	bool AABB(const AEVec2& bottom_left_1, const f32& width_1, const f32& height_1, const AEVec2& bottom_left_2, const f32& width_2, const f32& height_2);
	void SetDeltaTime(float dt);

}
#endif