#include "Utils.hpp"
#include "AEMath.h"
#include "../Entities/GameObjectEntity.hpp"
#include "AEOverload.hpp"
#include <functional>
#include <algorithm>
#include <vector>
#include <cmath>

struct OBBStruct {
	AEVec2 center;
	AEVec2 axes[2]; // Local X and Y unit vectors (normalized)
	f32 halfWidths[2]; // Half-extents along the axes
};

Color::Color(int a, int r, int g, int b) 
: a{ static_cast<unsigned char>(a) },
  r{ static_cast<unsigned char>(r) },
  g{ static_cast<unsigned char>(g) },
  b{ static_cast<unsigned char>(b) } {}

Color::Color(u32 packed)
: a{ static_cast<unsigned char>((packed >> 24) & 255) },
  r{ static_cast<unsigned char>((packed >> 16) & 255) }, 
  g{ static_cast<unsigned char>((packed >> 8) & 255) }, 
  b{ static_cast<unsigned char>(packed & 255) } {}

Color::Color(Color const& copy) 
: a{ copy.a }, r{ copy.r }, g{ copy.g }, b{ copy.b } {}

u32 Color::Pack() const {
	u32 color = (alpha << 24) + (red << 16) + (green << 8) + (blue);
	return color;
}

Color::Color(f32 a, f32 r, f32 g, f32 b)
: a{ static_cast<unsigned char>(255 * a) },
  r{ static_cast<unsigned char>(255 * r) },
  g{ static_cast<unsigned char>(255 * g) },
  b{ static_cast<unsigned char>(255 * b) } {}

std::istream& operator>> (std::istream& is, Color& color) {
	u32 packed;
	is >> packed;
	color = Color{ packed };
	return is;
}

std::ostream& operator<< (std::ostream& os, Color const& color) {
	return os << color.Pack();
}

Color operator-(Color const& lhs, Color const& rhs) {
	return Color{ lhs.a - rhs.a, lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b };
}

Color operator+(Color const& lhs, Color const& rhs) {
	return Color{ min(lhs.a + rhs.a, 255), min(lhs.r + rhs.r, 255), min(lhs.g + rhs.g, 255), min(lhs.b + rhs.b, 255) };
}

Color operator*(Color const& lhs, f32 scalar) {
	return Color{ min(lhs.a * scalar, 255), min(lhs.r * scalar, 255), min(lhs.g * scalar, 255), min(lhs.b * scalar, 255) };
}

Color operator*(f32 scalar, Color const& lhs) {
	return lhs * scalar;
}

namespace Utils {
	static f32 world_width = 48.f;
	static f32 world_height = 27.f;

	static f32 deltaTime;

	unsigned int screen_width = 1600;
	unsigned int screen_height = 900;

	f32 RandRange(f32 mn, f32 mx) {
		f32 temp = mn;
		mn = min(mn, mx);
		mx = max(temp, mx);
		return ((mx - mn) * AERandFloat()) + mn;
	}

	AEVec2 WorldToScreen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width),
			screen_height - y * (screen_height / world_height)
		};
	}

	AEVec2 ScreenToWorld(f32 x, f32 y) {
		return {
			x * (world_width / screen_width),
			world_height - y * (world_height / screen_height)
		};
	}

	AEVec2 GameToScreen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width) - (static_cast<f32>(screen_width) * 0.5f),
			y * (screen_height / world_height) - (static_cast<f32>(screen_height) * 0.5f)
		};
	}

	AEVec2 GameToTextScreen(f32 x, f32 y, bool cam) {
		if (cam) {
			f32 cam_x, cam_y;
			AEGfxGetCamPosition(&cam_x, &cam_y);
			AEVec2 screen = WorldToScreen(x, y);
			AEVec2 world = Utils::ScreenToWorld(screen.x - (cam ? cam_x : 0.f), screen.y + (cam ? cam_y : 0.f));
			x = world.x;
			y = world.y;
		}
		return {
			((x / world_width) * 2.f) - 1.f,
			((y / world_height) * 2.f) - 1.f
		};
	}

	AEVec2 ScreenSizeMultiplier() {
		return { screen_width / 1600.f, screen_height / 900.f };
	}

	AEVec2 ScaleToScreen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width),
			y * (screen_height / world_height)
		};
	}

	AEVec2 ScreenToScale(f32 x, f32 y) {
		return {
			x / (screen_width / world_width),
			y / (screen_height / world_height)
		};
	}

	std::pair<int, int> GetScreenResolution(void) {
		return { screen_width, screen_height };
	}

	void SetScreenResolution(unsigned int width, unsigned int height) {
		screen_width = width;
		screen_height = height;
	}

	const AEVec2 GetMouseWorld(bool cam) {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };

		f32 cam_x, cam_y;
		AEGfxGetCamPosition(&cam_x, &cam_y);
		return Utils::ScreenToWorld(mouse.x + (cam ? cam_x : 0.f), mouse.y - (cam ? cam_y : 0.f));
	}

	void GetMinMaxAABB(GameObjectEntity* const& go, AEVec2& min, AEVec2& max)
	{
		min = { go->prev_position.x - std::abs(go->scale.x) * 0.5f, go->prev_position.y - std::abs(go->scale.y) * 0.5f };
		max = { go->prev_position.x + std::abs(go->scale.x) * 0.5f, go->prev_position.y + std::abs(go->scale.y) * 0.5f };
	}

	const f32 GetWorldWidth(void) {
		return world_width;
	}

	const f32 GetWorldHeight(void) {
		return world_height;
	}

	const f32 GetDeltaTime(void) {
		return deltaTime;
	}

	bool CircleCircleCollision(BaseEntity* const& go, BaseEntity* const& go2) {
		f32 sqrDist = AEVec2SquareDistance(&go->position, &go2->position);
		f32 radii = max(std::abs(go->scale.x), std::abs(go->scale.y));
		f32 combinedRadii = radii + radii;
		return sqrDist <= combinedRadii * combinedRadii;
	}

	bool CircleOBBCollision(BaseEntity* const& circle, BaseEntity* const& aabb) {
		// get the boundaries of the aabb
		AEVec2 aabb_min = { -std::abs(aabb->scale.x) * 0.5f, -std::abs(aabb->scale.y) * 0.5f };
		AEVec2 aabb_max = { std::abs(aabb->scale.x) * 0.5f, std::abs(aabb->scale.y) * 0.5f};

		// circle location position in respect to the aabb
		AEVec2 r = circle->position - aabb->position;

		// rotate local position back to axis
		AEVec2 local_circle_pos;
		AEVec2Rotate(&local_circle_pos, &r, aabb->rotation);

		// AABB collision testing
		AEVec2 closest_point = AEVec2{ local_circle_pos };
		if (closest_point.x < aabb_min.x) {
			closest_point.x = aabb_min.x;
		}
		else if (closest_point.x > aabb_max.x) {
			closest_point.x = aabb_max.x;
		}

		if (closest_point.y < aabb_min.y) {
			closest_point.y = aabb_min.y;
		}
		else if (closest_point.y > aabb_max.y) {
			closest_point.y = aabb_max.y;
		}

		// get back the distance between the circle pos and the closest point
		// this is also the normal for collision response
		AEVec2 aabb_to_circle = local_circle_pos - closest_point;

		// now the actual collision checking, the distance between circle center 
		// and closest point to aabb is less than circle radius
		f32 radius = max(std::abs(circle->scale.x), std::abs(circle->scale.y)) * 0.5f;
		return AEVec2SquareLength(&aabb_to_circle) <= std::powf(radius, 2);
	}

	bool AABB(BaseEntity* const& go, BaseEntity* const& go2) {
		return !(go->position.x + std::abs(go->scale.x) * 0.5f < go2->position.x - std::abs(go2->scale.x) * 0.5f || go->position.x - std::abs(go->scale.x) * 0.5f > go2->position.x + std::abs(go2->scale.x) * 0.5f ||
			go->position.y + std::abs(go->scale.y) * 0.5f < go2->position.y - std::abs(go2->scale.y) * 0.5f || go->position.y - std::abs(go->scale.y) * 0.5f > go2->position.y + std::abs(go2->scale.y) * 0.5f);
	}

	bool DynamicAABB(GameObjectEntity* const& go, GameObjectEntity* const& go2, float& tCollision, const float& dt)
	{
		AEVec2 minGO, maxGO, minGO2, maxGO2;
		GetMinMaxAABB(go, minGO, maxGO);
		GetMinMaxAABB(go2, minGO2, maxGO2);


		float tFirst = 0.0f;
		float tLast = dt;
		AEVec2 bRelative = { go2->velocity.x - go->velocity.x, go2->velocity.y - go->velocity.y };
		if (bRelative.x < 0) {
			if (minGO.x > maxGO2.x)
				return false;
			if (maxGO.x < minGO2.x)
				tFirst = AEMax((maxGO.x - minGO2.x) / bRelative.x, tFirst);
			if (minGO.x < maxGO2.x)
				tLast = AEMin((minGO.x - maxGO2.x) / bRelative.x, tLast);
		}
		else if (bRelative.x > 0) {
			if (maxGO.x < minGO2.x)
				return false;
			if (minGO.x > maxGO2.x) {
				tFirst = AEMax((minGO.x - maxGO2.x) / bRelative.x, tFirst);
			}
			if (maxGO.x > minGO2.x) {
				tLast = AEMin((maxGO.x - minGO2.x) / bRelative.x, tLast);
			}
		}
		else {
			if (maxGO.x < minGO2.x)
				return false;
			else if (minGO.x > maxGO2.x)
				return false;
		}

		if (tFirst > tLast)
			return false;
		if (bRelative.y < 0) {
			if (minGO.y > maxGO2.y)
				return false;
			if (maxGO.y < minGO2.y)
				tFirst = AEMax((maxGO.y - minGO2.y) / bRelative.y, tFirst);
			if (minGO.y < maxGO2.y)
				tLast = AEMin((minGO.y - maxGO2.y) / bRelative.y, tLast);
		}
		else if (bRelative.y > 0) {
			if (minGO.y > maxGO2.y) {
				tFirst = AEMax((minGO.y - maxGO2.y) / bRelative.y, tFirst);
			}
			if (maxGO.y > minGO2.y) {
				tLast = AEMin((maxGO.y - minGO2.y) / bRelative.y, tLast);
			}
			if (maxGO.y < minGO2.y)
				return false;
		}
		else {
			if (maxGO.y < minGO2.y)
				return false;
			else if (minGO.y > maxGO2.y)
				return false;
		}

		if (tFirst > tLast)
			return false;
		tCollision = tFirst;
		return true;
	}

	//bool DynamicAABB(const BaseEntity* const& go, const BaseEntity* const& go2, AEVec2& contact, AEVec2& normal, float& tCollision, const f32& dt)
	//{
	//	AEVec2 relativeVel{ go->velocity - go2->velocity };
	//	if (relativeVel == 0.f) {
	//		return false;
	//	}
	//	//BaseEntity target = *go2;
	//	//target.scale += go->scale;
	//	
	//	if (RayAABB(go->position, go->velocity * dt, go2, contact, normal, tCollision)) {
	//		if (tCollision <= 1.0f) {
	//			return true;
	//		}
	//	}
	//	return false;
	//}

	 bool RayAABB(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target, AEVec2& contact, AEVec2& normal, float& tNear)
	{
		AEVec2 topLeft = { target->position.x - std::abs(target->scale.x) * 0.5f, target->position.y + std::abs(target->scale.y) * 0.5f };
		AEVec2 bottomRight = { target->position.x + std::abs(target->scale.x) * 0.5f, target->position.y - std::abs(target->scale.y) * 0.5f };
		AEVec2 tFirst = (topLeft - ray_origin) / ray_dir; // Contains Near X & Near Y
		AEVec2 tLast = (bottomRight - ray_origin) / ray_dir; // Contains Far X & Far Y


		if (std::isnan(tFirst.x) || std::isnan(tFirst.y)) return false;
		if (std::isnan(tLast.x) || std::isnan(tLast.y)) return false;


		if (tFirst.x > tLast.x) std::swap(tFirst.x, tLast.x);
		if (tFirst.y > tLast.y) std::swap(tFirst.y, tLast.y);

		if (tFirst.x > tLast.y || tFirst.y > tLast.x) return false;

		tNear = AEMax(tFirst.x, tFirst.y);
		float tFar = AEMin(tLast.x, tLast.y);

		if (tFar < 0) return false;

		contact = ray_origin + ray_dir * tNear;

		if (tFirst.x > tFirst.y) {
			if (ray_dir.x < 0)
				normal = { 1.f, 0.f };
			else
				normal = { -1.f, 0.f };
		}
		else if (tFirst.x < tFirst.y) {
			if (ray_dir.y < 0)
				normal = { 0.f, 1.f };
			else
				normal = { 0.f, -1.f };
		}
		return true;
	}

	 bool RayHitAny(const AEVec2& ray_origin, const AEVec2& ray_dir, std::vector<BaseEntity*> const& entities)
	 {
		 AEVec2 contact, normal;
		 float tCollide;
		 for (const BaseEntity* const& entity : entities) {
			 if (RayAABB(ray_origin, ray_dir, entity, contact, normal, tCollide) && tCollide <= 1.f)
				 return true;
		 }
		 return false;
	 }

	 bool RayHit(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target)
	 {
		 AEVec2 contact, normal;
		 float tCollide;
		 return RayAABB(ray_origin, ray_dir, target, contact, normal, tCollide) && tCollide < 1.f;
	 }

	std::vector<AEVec2> GetCorners(const BaseEntity* go) {
		std::vector<AEVec2> corners(4);
		f32 cosA = AECos(go->rotation);
		f32 sinA = AESin(go->rotation);

		// Local axes vectors
		AEVec2 dirX = { cosA, sinA };
		AEVec2 dirY = { -sinA, cosA };

		// Combine center with scaled axes
		// top right
		corners[0] = { go->position.x + dirX.x * (std::abs(go->scale.x) * 0.5f) + dirY.x * (std::abs(go->scale.y) * 0.5f), go->position.y + dirX.y * (std::abs(go->scale.x) * 0.5f) + dirY.y * (std::abs(go->scale.y) * 0.5f) };

		// bottom right
		corners[1] = { go->position.x - dirX.x * (std::abs(go->scale.x) * 0.5f) + dirY.x * (std::abs(go->scale.y) * 0.5f), go->position.y - dirX.y * (std::abs(go->scale.x) * 0.5f) + dirY.y * (std::abs(go->scale.y) * 0.5f) };

		// bottom left
		corners[2] = { go->position.x - dirX.x * (std::abs(go->scale.x) * 0.5f) - dirY.x * (std::abs(go->scale.y) * 0.5f), go->position.y - dirX.y * (std::abs(go->scale.x) * 0.5f) - dirY.y * (std::abs(go->scale.y) * 0.5f) };

		// top left
		corners[3] = { go->position.x + dirX.x * (std::abs(go->scale.x) * 0.5f) - dirY.x * (std::abs(go->scale.y) * 0.5f), go->position.y + dirX.y * (std::abs(go->scale.x) * 0.5f) - dirY.y * (std::abs(go->scale.y) * 0.5f) };

		return corners;
	}

	bool OBB(BaseEntity* const& go, BaseEntity* const& go2) {
		// Initial broader phase collision check
		auto cornersA = GetCorners(go);
		auto cornersB = GetCorners(go2);

		// Axes to check: the normals of the sides of both rectangles
		// For a rectangle, we only need 2 axes per box (perpendicular sides)
		std::vector<AEVec2> axes = {
			{ AECos(go->rotation), -AESin(go->rotation) },  // Box A Local X
			{ AESin(go->rotation), AECos(go->rotation) }, // Box A Local Y
			{ AECos(go2->rotation), -AESin(go2->rotation) },  // Box B Local X
			{ AESin(go2->rotation), AECos(go2->rotation) }  // Box B Local Y
		};

		for (auto& axis : axes) {
			f32 minA = AEVec2DotProduct(&cornersA[0], &axis);
			f32 maxA = minA;
			for (size_t i = 1; i < 4; ++i) {
				f32 p = AEVec2DotProduct(&cornersA[i], &axis);
				if (p < minA) minA = p;
				if (p > maxA) maxA = p;
			}

			// Project corners of B
			f32 minB = AEVec2DotProduct(&cornersB[0], &axis);
			f32 maxB = minB;
			for (size_t i = 1; i < 4; ++i) {
				f32 p = AEVec2DotProduct(&cornersB[i], &axis);
				if (p < minB) minB = p;
				if (p > maxB) maxB = p;
			}

			// SAT Gap Check: If the projections on this axis don't overlap, there's no collision
			if (maxA < minB || maxB < minA) {
				return false;
			}
		}

		return true; // Overlap on all axes means a collision
	}

	bool OBBPoint(BaseEntity* const& go, AEVec2 const& pos) {
		AEVec2 local_pos = pos - go->position;
		//AEVec2Rotate(&local_pos, &local_pos, go->rotation);

		return (local_pos.x <= std::abs(go->scale.x) * 0.5f && local_pos.x >= -std::abs(go->scale.x) * 0.5f &&
			local_pos.y <= std::abs(go->scale.y) * 0.5f && local_pos.y >= -std::abs(go->scale.y) * 0.5f);
	}

	void SetDeltaTime(f32 dt) {
		deltaTime = dt;
	}

	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2)
	{
		if (std::abs(vec2->x) > std::abs(vec2->y))
		{
			result->x = vec2->x / std::abs(vec2->x);
			result->y = 0.0;
		}
		else
		{
			result->x = 0.0;
			result->y = vec2->y / std::abs(vec2->y);
		}
	}

	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot) {
		AEMtx33 scale{ 1.f };
		AEVec2 s_scale = Utils::ScaleToScreen(sca.x, sca.y);
		AEMtx33Identity(&scale);
		AEMtx33Scale(&scale, s_scale.x, s_scale.y);
		AEMtx33 rotate = { 0 };
		AEMtx33Identity(&rotate);
		AEMtx33Rot(&rotate, rot);
		AEVec2 screenPos = Utils::GameToScreen(pos.x, pos.y);
		AEMtx33 translate = { 0 };
		AEMtx33Identity(&translate);
		AEMtx33Trans(&translate, screenPos.x, screenPos.y);
		return translate * rotate * scale;
	}
}