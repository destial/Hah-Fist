/*!
* @file Utils.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Definition file for utility functions
*/

#include "Utils.hpp"
#include "AEMath.h"
#include "../Entities/GameObjectEntity.hpp"
#include "AEOverload.hpp"
#include <functional>
#include <algorithm>
#include <vector>
#include <cmath>

struct OBBStruct {
	AEVec2 center; // center of OBB
	AEVec2 axes[2]; // Local X and Y unit vectors (normalized)
	f32 halfWidths[2]; // Half-extents along the axes
};

Color::Color(int a, int r, int g, int b) 
: a{ static_cast<unsigned char>(a) },
  r{ static_cast<unsigned char>(r) },
  g{ static_cast<unsigned char>(g) },
  b{ static_cast<unsigned char>(b) } {} // Ctor

Color::Color(u32 packed)
: a{ static_cast<unsigned char>((packed >> 24) & 255) },
  r{ static_cast<unsigned char>((packed >> 16) & 255) }, 
  g{ static_cast<unsigned char>((packed >> 8) & 255) }, 
  b{ static_cast<unsigned char>(packed & 255) } {} // Ctor

Color::Color(Color const& copy) 
: a{ copy.a }, r{ copy.r }, g{ copy.g }, b{ copy.b } {} // Ctor

Color::Color(f32 a, f32 r, f32 g, f32 b)
: a{ static_cast<unsigned char>(255 * a) },
  r{ static_cast<unsigned char>(255 * r) },
  g{ static_cast<unsigned char>(255 * g) },
  b{ static_cast<unsigned char>(255 * b) } {} // Ctor

/*!
* @brief Pack the colors into a 32-bit integer
* @brief 8-bits per channel stored as [ARGB]
* @return The packed integer
*/
u32 Color::Pack() const {
	u32 packed = (alpha << 24) + (red << 16) + (green << 8) + (blue);
	return packed;
}

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
	return Color{ 
		max(static_cast<int>(lhs.a) - rhs.a, 0),
		max(static_cast<int>(lhs.r) - rhs.r, 0),
		max(static_cast<int>(lhs.g) - rhs.g, 0),
		max(static_cast<int>(lhs.b) - rhs.b, 0)
	};
}

Color operator+(Color const& lhs, Color const& rhs) {
	return Color{ 
		min(static_cast<int>(lhs.a) + rhs.a, 255),
		min(static_cast<int>(lhs.r) + rhs.r, 255),
		min(static_cast<int>(lhs.g) + rhs.g, 255),
		min(static_cast<int>(lhs.b) + rhs.b, 255)
	};
}

Color operator*(Color const& lhs, f32 scalar) {
	return Color{ 
		min(static_cast<int>(lhs.a) * scalar, 255),
		min(static_cast<int>(lhs.r) * scalar, 255),
		min(static_cast<int>(lhs.g) * scalar, 255),
		min(static_cast<int>(lhs.b) * scalar, 255)
	};
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

	/*!
	* @brief Get a random number between two ranges
	* @param mn - Minimum range
	* @param mx - Maximum range
	* @return A float between a range
	*/
	f32 RandRange(f32 mn, f32 mx) {
		f32 temp = mn;
		mn = min(mn, mx);
		mx = max(temp, mx);
		return ((mx - mn) * AERandFloat()) + mn;
	}

	/*!
	* @brief Convert world coordinates to screen coordinates
	* @param x - World x
	* @param y - World y
	* @return Vec2 of screen coordinates
	*/
	AEVec2 WorldToScreen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width),
			screen_height - y * (screen_height / world_height)
		};
	}

	/*!
	* @brief Convert screen coordinates to world coordinates
	* @param x - Screen x
	* @param y - Screen y
	* @return Vec2 of world coordinates
	*/
	AEVec2 ScreenToWorld(f32 x, f32 y) {
		return {
			x * (world_width / screen_width),
			world_height - y * (world_height / screen_height)
		};
	}

	/*!
	* @brief Convert game coordinates to screen coordinates
	* @param x - Game x
	* @param y - Game y
	* @return Vec2 of screen coordinates
	*/
	AEVec2 GameToScreen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width) - (static_cast<f32>(screen_width) * 0.5f),
			y * (screen_height / world_height) - (static_cast<f32>(screen_height) * 0.5f)
		};
	}

	/*!
	* @brief Convert game coordinates to text coordinates
	* @param x - Game x
	* @param y - Game y
	* @return Vec2 of text coordinates
	*/
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

	/*!
	* @brief Get the screen size multiplier (e.g. fullscreen or different from 1600x900)
	* @return Vec2 of size multiplier (x,y)
	*/
	AEVec2 ScreenSizeMultiplier() {
		return { screen_width / 1600.f, screen_height / 900.f };
	}

	/*!
	* @brief Convert scales to screen size
	* @param x - Scale x
	* @param y - Scale y
	* @return Vec2 of screen size
	*/
	AEVec2 ScaleToScreen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width),
			y * (screen_height / world_height)
		};
	}

	/*!
	* @brief Convert screen size to scale
	* @param x - Screen x
	* @param y - Screen y
	* @return Vec2 of scale size
	*/
	AEVec2 ScreenToScale(f32 x, f32 y) {
		return {
			x / (screen_width / world_width),
			y / (screen_height / world_height)
		};
	}

	/*!
	* @brief Get the screen resolution
	* @return The screen resolution (first = x, second = y)
	*/
	std::pair<int, int> GetScreenResolution(void) {
		return { screen_width, screen_height };
	}

	/*!
	* @brief Set the current screen resolution
	* @param width - Screen width
	* @param height - Screen height
	*/
	void SetScreenResolution(unsigned int width, unsigned int height) {
		screen_width = width;
		screen_height = height;
	}

	/*!
	* @brief Get the mouse coordinates in world space
	* @param camera - Take into account camera position or not
	* @return Vec2 of mouse coordinates
	*/
	const AEVec2 GetMouseWorld(bool cam) {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };

		f32 cam_x, cam_y;
		AEGfxGetCamPosition(&cam_x, &cam_y);
		return Utils::ScreenToWorld(mouse.x + (cam ? cam_x : 0.f), mouse.y - (cam ? cam_y : 0.f));
	}

	/*!
	* @brief Get the min max AABB of a game object
	* @param go - The game object
	* @param min - Output the min
	* @param max - Output the max
	*/
	void GetMinMaxAABB(GameObjectEntity* const& go, AEVec2& min, AEVec2& max) {
		min = { go->prev_position.x - std::fabs(go->scale.x) * 0.5f, go->prev_position.y - std::fabs(go->scale.y) * 0.5f };
		max = { go->prev_position.x + std::fabs(go->scale.x) * 0.5f, go->prev_position.y + std::fabs(go->scale.y) * 0.5f };
	}

	/*!
	* @brief Get the world width
	* @return The world width
	*/
	const f32 GetWorldWidth(void) {
		return world_width;
	}

	/*!
	* @brief Get the world height
	* @return The world height
	*/
	const f32 GetWorldHeight(void) {
		return world_height;
	}

	/*!
	* @brief Get the current game delta time
	*/
	const f32 GetDeltaTime(void) {
		return deltaTime;
	}

	/*!
	* @brief Set the current game delta time
	* @param dt - The delta time
	*/
	void SetDeltaTime(f32 dt) {
		deltaTime = dt;
	}

	/*!
	* @brief Test collision between two circle entities
	* @param en - The first entity
	* @param en2 - The other entity
	* @return true if overlap
	*/
	bool CircleCircleCollision(BaseEntity* const& go, BaseEntity* const& go2) {
		f32 sqrDist = AEVec2SquareDistance(&go->position, &go2->position);
		f32 radii = max(std::fabs(go->scale.x), std::fabs(go->scale.y));
		f32 combinedRadii = radii + radii;
		return sqrDist <= combinedRadii * combinedRadii;
	}

	/*!
	* @brief Test collision between circle and OBB entity
	* @param circle - The circle entity
	* @param obb - The obb entity
	* @return true if overlap
	*/
	bool CircleOBBCollision(BaseEntity* const& circle, BaseEntity* const& aabb) {
		// Get the boundaries of the aabb
		AEVec2 aabb_min = { -std::fabs(aabb->scale.x) * 0.5f, -std::fabs(aabb->scale.y) * 0.5f };
		AEVec2 aabb_max = { std::fabs(aabb->scale.x) * 0.5f, std::fabs(aabb->scale.y) * 0.5f};

		// Circle location position in respect to the aabb
		AEVec2 r = circle->position - aabb->position;

		// Rotate local position back to axis
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

		// Get back the distance between the circle pos and the closest point
		// this is also the normal for collision response
		AEVec2 aabb_to_circle = local_circle_pos - closest_point;

		// Now the actual collision checking, the distance between circle center 
		// and closest point to aabb is less than circle radius
		f32 radius = max(std::fabs(circle->scale.x), std::fabs(circle->scale.y)) * 0.5f;
		return AEVec2SquareLength(&aabb_to_circle) <= std::powf(radius, 2);
	}

	/*!
	* @brief Test collision between two AABB entities
	* @param en - The first entity
	* @param en2 - The other entity
	* @return true if overlap
	*/
	bool AABB(BaseEntity* const& go, BaseEntity* const& go2) {
		return !(go->position.x + std::fabs(go->scale.x) * 0.5f < go2->position.x - std::fabs(go2->scale.x) * 0.5f || go->position.x - std::fabs(go->scale.x) * 0.5f > go2->position.x + std::fabs(go2->scale.x) * 0.5f ||
			go->position.y + std::fabs(go->scale.y) * 0.5f < go2->position.y - std::fabs(go2->scale.y) * 0.5f || go->position.y - std::fabs(go->scale.y) * 0.5f > go2->position.y + std::fabs(go2->scale.y) * 0.5f);
	}

	/*!
	* @brief Test collision between two dynamic AABB game entities
	* @param go - The first entity
	* @param go2 - The other entity
	* @param tCollision - Output the time of collision
	* @param dt - The delta time
	* @return true if collision
	*/
	bool DynamicAABB(GameObjectEntity* const& go, GameObjectEntity* const& go2, float& tCollision, const float& dt) {
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

	/*!
	* @brief Test collision between a ray and a AABB
	* @param ray_origin - The origin of the ray
	* @param ray_dir - The direction of the ray
	* @param target - The AABB entity to check against
	* @param contact - Output the contact point
	* @param normal - Output the normal line
	* @param tCollision - Output the time of collision
	* @param true if hit
	*/
	bool RayAABB(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target, AEVec2& contact, AEVec2& normal, float& tNear) {
		AEVec2 topLeft = { target->position.x - std::fabs(target->scale.x) * 0.5f, target->position.y + std::fabs(target->scale.y) * 0.5f };
		AEVec2 bottomRight = { target->position.x + std::fabs(target->scale.x) * 0.5f, target->position.y - std::fabs(target->scale.y) * 0.5f };
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

	/*!
	* @brief Test collision between ray and every entities
	* @brief This method relies on RayAABB to check if It collides with any of the provided std::vector of entities.
	* @brief If it collides with just one entity in a list of entities, it will return true
	* @param ray_origin - The origin of the ray
	* @param ray_dir - The direction of the ray
	* @param entities - The vector of entities provided
	* @param true if any of them were hit
	*/
	bool RayHitAny(const AEVec2& ray_origin, const AEVec2& ray_dir, std::vector<BaseEntity*> const& entities) {
		AEVec2 contact, normal;
		float tCollide;
		for (const BaseEntity* const& entity : entities) {
			if (RayAABB(ray_origin, ray_dir, entity, contact, normal, tCollide) && tCollide <= 1.f)
				return true;
		}
		return false;
	}

	/*!
	* @brief Test collision between a ray and an entity
	* @param ray_origin - The origin of the ray
	* @param ray_dir - The direction of the ray
	* @param target - The AABB entity to check against
	* @param true if hit
	*/
	bool RayHit(const AEVec2& ray_origin, const AEVec2& ray_dir, const BaseEntity* const& target) {
		AEVec2 contact, normal;
		float tCollide;
		return RayAABB(ray_origin, ray_dir, target, contact, normal, tCollide) && tCollide < 1.f;
	}

	/*!
	* @brief Get the coordinates of each corner of a game object
	* @param go - The game object
	* @param A vector of the corners of the game object (currently 4 corners)
	*/
	std::vector<AEVec2> GetCorners(const BaseEntity* go) {
		std::vector<AEVec2> corners(4);
		f32 cosA = AECos(go->rotation);
		f32 sinA = AESin(go->rotation);

		// Local axes vectors
		AEVec2 dirX = { cosA, sinA };
		AEVec2 dirY = { -sinA, cosA };

		// Combine center with scaled axes
		// Top right
		corners[0] = { go->position.x + dirX.x * (std::fabs(go->scale.x) * 0.5f) + dirY.x * (std::fabs(go->scale.y) * 0.5f), go->position.y + dirX.y * (std::fabs(go->scale.x) * 0.5f) + dirY.y * (std::fabs(go->scale.y) * 0.5f) };

		// Bottom right
		corners[1] = { go->position.x - dirX.x * (std::fabs(go->scale.x) * 0.5f) + dirY.x * (std::fabs(go->scale.y) * 0.5f), go->position.y - dirX.y * (std::fabs(go->scale.x) * 0.5f) + dirY.y * (std::fabs(go->scale.y) * 0.5f) };

		// Bottom left
		corners[2] = { go->position.x - dirX.x * (std::fabs(go->scale.x) * 0.5f) - dirY.x * (std::fabs(go->scale.y) * 0.5f), go->position.y - dirX.y * (std::fabs(go->scale.x) * 0.5f) - dirY.y * (std::fabs(go->scale.y) * 0.5f) };

		// Top left
		corners[3] = { go->position.x + dirX.x * (std::fabs(go->scale.x) * 0.5f) - dirY.x * (std::fabs(go->scale.y) * 0.5f), go->position.y + dirX.y * (std::fabs(go->scale.x) * 0.5f) - dirY.y * (std::fabs(go->scale.y) * 0.5f) };

		return corners;
	}

	/*!
	* @brief Test collision between two OBB entities
	* @param en - The first entity
	* @param en2 - The other entity
	* @return true if overlap
	*/
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

	/*!
	* @brief Test collision between a point and OBB entity
	* @param en - The first entity
	* @param pos - The point to check
	* @return true if point inside
	*/
	bool OBBPoint(BaseEntity* const& go, AEVec2 const& pos) {
		AEVec2 local_pos = pos - go->position;
		AEVec2Rotate(&local_pos, &local_pos, go->rotation);

		return (local_pos.x <= std::fabs(go->scale.x) * 0.5f && local_pos.x >= -std::fabs(go->scale.x) * 0.5f &&
			local_pos.y <= std::fabs(go->scale.y) * 0.5f && local_pos.y >= -std::fabs(go->scale.y) * 0.5f);
	}

	/*!
	* @brief Snap a vector to a smooth axis
	* @param result - Output the resulting snapped vector
	* @param vec2 - The vector to snap
	*/
	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2) {
		if (std::fabs(vec2->x) > std::fabs(vec2->y)) {
			result->x = vec2->x / std::fabs(vec2->x);
			result->y = 0.0;
		}
		else {
			result->x = 0.0;
			result->y = vec2->y / std::fabs(vec2->y);
		}
	}

	/*!
	* @brief Get a transformation matrix based on vector positions, scale and radian rotation
	* @param pos - Position
	* @param sca - Scale
	* @param rot - Rotation (in radians)
	* @return The transformation matrix (T * R * S)
	*/
	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot) {
		// Scale matrix
		AEMtx33 scale{ 1.f };
		AEVec2 s_scale = Utils::ScaleToScreen(sca.x, sca.y);
		AEMtx33Identity(&scale);
		AEMtx33Scale(&scale, s_scale.x, s_scale.y);

		// Rotation matrix
		AEMtx33 rotate = { 0 };
		AEMtx33Identity(&rotate);
		AEMtx33Rot(&rotate, rot);

		// Translation matrix
		AEVec2 screenPos = Utils::GameToScreen(pos.x, pos.y);
		AEMtx33 translate = { 0 };
		AEMtx33Identity(&translate);
		AEMtx33Trans(&translate, screenPos.x, screenPos.y);

		// T * R * S
		return translate * rotate * scale;
	}
}