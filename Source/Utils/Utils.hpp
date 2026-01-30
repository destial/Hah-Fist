#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"
#include <vector>

namespace Game {
	void SetGameRunning(bool b);
}

class BaseEntity;

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
	AEVec2 World_To_Screen(f32 x, f32 y);
	AEVec2 Screen_To_World(f32 x, f32 y);
	AEVec2 Game_To_Screen(f32 x, f32 y);
	AEVec2 Scale_To_Screen(f32 x, f32 y);
	AEVec2 Screen_To_Scale(f32 x, f32 y);
	AEVec2 Game_To_TextScreen(f32 x, f32 y);

	const AEVec2 Get_Mouse_World(bool camera = false);

	u32 PackColor(int red, int green, int blue, int alpha = 255);
	u32 PackColor(Color& color);
	Color UnpackColor(u32 color);

	const f32 GetWorldWidth(void);
	const f32 GetWorldHeight(void);
	const f32 GetDeltaTime(void);

	bool CircleCircleCollision(BaseEntity*& go, BaseEntity*& go2);
	//bool CircleCircleCollision(AEVec2& center_pos_1, const f32& radius_1, AEVec2& center_pos_2, const f32& radius_2);
	//bool AABB(const AEVec2& bottom_left_1, const f32& width_1, const f32& height_1, const AEVec2& bottom_left_2, const f32& width_2, const f32& height_2);
	std::vector<AEVec2> GetCorners(const BaseEntity* go);
	bool AABB(const BaseEntity* const & go,  const BaseEntity* const& go2);
	bool OBB(const BaseEntity* const& go, const BaseEntity* const& go2);
	bool OBBPoint(const BaseEntity* const& go, AEVec2 const& pos);
	
	void SetDeltaTime(f32 dt);

	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2);

	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot);
}
#endif