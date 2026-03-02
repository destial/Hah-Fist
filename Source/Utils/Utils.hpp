#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"
#include <iostream>
#include <vector>
#include <utility>

#define min_max(v, mn, mx) min(mx, max(mn, v))

class BaseEntity;

struct Color {
	Color(u32 packed = 0);
	Color(int a, int r, int g, int b);
	Color(f32 a, f32 r, f32 g, f32 b);
	Color(Color const& copy);

	u32 Pack() const;

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

std::istream& operator>> (std::istream& is, Color& color);

std::ostream& operator<< (std::ostream& os, Color const& color);

namespace Game {
	void SetGameRunning(bool b);
	void SetBackgroundColor(Color c);
}

namespace Utils {
	AEVec2 WorldToScreen(f32 x, f32 y);
	AEVec2 ScreenToWorld(f32 x, f32 y);
	AEVec2 GameToScreen(f32 x, f32 y);
	AEVec2 ScaleToScreen(f32 x, f32 y);
	AEVec2 ScreenToScale(f32 x, f32 y);
	AEVec2 GameToTextScreen(f32 x, f32 y);

	f32 RandRange(f32 mn, f32 mx);
	const f32 GetWorldWidth(void);
	const f32 GetWorldHeight(void);
	const f32 GetDeltaTime(void);
	std::pair<int, int> GetScreenResolution(void);
	void SetScreenResolution(unsigned int width, unsigned int height);
	void SetDeltaTime(f32 dt);

	const AEVec2 GetMouseWorld(bool camera = false);

	std::vector<AEVec2> GetCorners(const BaseEntity* go);
	bool CircleCircleCollision(BaseEntity*& go, BaseEntity*& go2);
	bool AABB(const BaseEntity* const & go,  const BaseEntity* const& go2);
	bool DynamicAABB(const BaseEntity* const& go, const BaseEntity* const& go2, AEVec2& contact, AEVec2& normal, float& tCollision, const f32& dt);
	bool RayAABB(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target, AEVec2& contact, AEVec2& normal, float& tCollision);
	bool OBB(const BaseEntity* const& go, const BaseEntity* const& go2);
	bool OBBPoint(const BaseEntity* const& go, AEVec2 const& pos);
	
	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2);

	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot);

	template<typename N>
	N Lerp(N start, N end, f32 delta) {
		return start + min_max(delta, 0.f, 1.f) * (end - start);
	}

	template<typename N>
	N LerpCircle(N start, N end, f32 delta) {
		delta = min_max(delta, 0.f, 1.f);
		f32 p = 1.f - ((AECos(PI * delta) + 1.f) * 0.5f);
		return Lerp<N>(start, end, p);
	}
}
#endif