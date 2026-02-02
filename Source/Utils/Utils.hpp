#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"
#include <vector>
#include <utility>

#define min_max(v, mn, mx) min(mx, max(mn, v))

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
	AEVec2 WorldToScreen(f32 x, f32 y);
	AEVec2 ScreenToWorld(f32 x, f32 y);
	AEVec2 GameToScreen(f32 x, f32 y);
	AEVec2 ScaleToScreen(f32 x, f32 y);
	AEVec2 ScreenToScale(f32 x, f32 y);
	AEVec2 GameToTextScreen(f32 x, f32 y);

	const f32 GetWorldWidth(void);
	const f32 GetWorldHeight(void);
	const f32 GetDeltaTime(void);
	std::pair<int, int> GetScreenResolution(void);
	void SetScreenResolution(unsigned int width, unsigned int height);
	void SetDeltaTime(f32 dt);

	const AEVec2 GetMouseWorld(bool camera = false);

	u32 PackColor(int red, int green, int blue, int alpha = 255);
	u32 PackColor(Color const& color);
	Color UnpackColor(u32 color);

	std::vector<AEVec2> GetCorners(const BaseEntity* go);
	bool CircleCircleCollision(BaseEntity*& go, BaseEntity*& go2);
	bool AABB(const BaseEntity* const & go,  const BaseEntity* const& go2);
	bool OBB(const BaseEntity* const& go, const BaseEntity* const& go2);
	bool OBBPoint(const BaseEntity* const& go, AEVec2 const& pos);
	
	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2);

	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot);

	f32 Lerp(f32 start, f32 end, f32 delta);
	int Lerp(int start, int end, f32 delta);
	f32 LerpCircle(f32 start, f32 end, f32 delta);
	int LerpCircle(int start, int end, f32 delta);
}
#endif