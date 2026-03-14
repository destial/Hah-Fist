/*!
* @file Utils.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Declaration file for a particle system within a scene
*/

#pragma once
#ifndef __UTIL_H
#define __UTIL_H
#include "AEEngine.h"
#include <iostream>
#include <vector>
#include <utility>

#define min_max(v, mn, mx) min(mx, max(mn, v))

class BaseEntity;
class GameObjectEntity;
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
	AEAudioGroup const& GetMusicGroup();
	AEAudioGroup const& GetSfxGroup();
}

namespace Utils {
	AEVec2 WorldToScreen(f32 x, f32 y);
	AEVec2 ScreenToWorld(f32 x, f32 y);
	AEVec2 GameToScreen(f32 x, f32 y);
	AEVec2 ScaleToScreen(f32 x, f32 y);
	AEVec2 ScreenToScale(f32 x, f32 y);
	AEVec2 GameToTextScreen(f32 x, f32 y, bool cam = false);

	f32 RandRange(f32 mn, f32 mx);
	const f32 GetWorldWidth(void);
	const f32 GetWorldHeight(void);
	const f32 GetDeltaTime(void);
	std::pair<int, int> GetScreenResolution(void);
	void SetScreenResolution(unsigned int width, unsigned int height);
	void SetDeltaTime(f32 dt);

	const AEVec2 GetMouseWorld(bool camera = false);
	void GetMinMaxAABB(GameObjectEntity* const& go, AEVec2& min,AEVec2& max);
	std::vector<AEVec2> GetCorners(const BaseEntity* go);
	bool CircleCircleCollision(BaseEntity* const& go, BaseEntity* const& go2);
	bool AABB(BaseEntity* const& go, BaseEntity* const& go2);
	bool DynamicAABB(GameObjectEntity* const& go, GameObjectEntity* const& go2, float& tCollision, const float& dt);
	//bool DynamicAABB(const BaseEntity* const& go, const BaseEntity* const& go2, AEVec2& contact, AEVec2& normal, float& tCollision, const f32& dt);
	// Ray vs AABB
	bool RayAABB(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target, AEVec2& contact, AEVec2& normal, float& tCollision);
	
	// This method relies on RayAABB to check if It collides with any of the provided std::vector of entities.
	// If it collides with just one entity in a list of entities, it will return true
	bool RayHitAny(const AEVec2& ray_origin, const AEVec2& ray_dir, std::vector<BaseEntity*> const& entities); 

	bool RayHit(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target);

	bool OBB(BaseEntity* const& go, BaseEntity* const& go2);
	bool OBBPoint(BaseEntity* const& go, AEVec2 const& pos);
	
	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2);

	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot);

	template<typename N>
	N Lerp(N start, N end, f32 delta) {
		return start + AEClamp(delta, 0.f, 1.f) * (end - start);
	}

	template<typename N>
	N LerpCircle(N start, N end, f32 delta) {
		delta = AEClamp(delta, 0.f, 1.f);
		f32 p = 1.f - ((AECos(PI * delta) + 1.f) * 0.5f);
		return Lerp(start, end, p);
	}
}
#endif