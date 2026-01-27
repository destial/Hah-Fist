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
	float halfWidths[2]; // Half-extents along the axes
};

namespace Utils {
	static float world_width = 48.f;
	static float world_height = 27.f;

	static float deltaTime;

	unsigned int screen_width = 1600;
	unsigned int screen_height = 900;

	AEVec2 World_To_Screen(float x, float y) {
		return {
			x * (screen_width / world_width),
			screen_height - y * (screen_height / world_height)
		};
	}

	AEVec2 Screen_To_World(float x, float y) {
		return {
			x * (world_width / screen_width),
			world_height - y * (world_height / screen_height)
		};
	}

	AEVec2 Game_To_Screen(float x, float y) {
		return {
			x * (screen_width / world_width) - (static_cast<f32>(screen_width) * 0.5f),
			y * (screen_height / world_height) - (static_cast<f32>(screen_height) * 0.5f)
		};
	}

	AEVec2 Game_To_TextScreen(float x, float y) {
		return {
			((x / world_width) * 2.f) - 1.f,
			((y / world_height) * 2.f) - 1.f
		};
	}

	AEVec2 Scale_To_Screen(float x, float y) {
		return {
			x * (screen_width / world_width),
			y * (screen_height / world_height)
		};
	}

	const AEVec2 Get_Mouse_World() {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
		return Utils::Screen_To_World(mouse.x, mouse.y);
	}

	u32 PackColor(int red, int green, int blue, int alpha) {
		u32 color = (alpha << 24) + (red << 16) + (green << 8) + (blue);
		return color;
	}

	u32 PackColor(Color& color) {
		u32 packed = (color.a << 24) + (color.r << 16) + (color.g << 8) + (color.b);
		return packed;
	}

	Color UnpackColor(u32 color) {
		Color c{ 0, 0, 0, 0 };
		c.a = static_cast<unsigned char>((color >> 24) & 255);
		c.r = static_cast<unsigned char>((color >> 16) & 255);
		c.g = static_cast<unsigned char>((color >> 8) & 255);
		c.b = static_cast<unsigned char>(color & 255);
		return c;
	}

	const float GetWorldWidth(void) {
		return world_width;
	}

	const float GetWorldHeight(void) {
		return world_height;
	}

	const float GetDeltaTime(void) {
		return deltaTime;
	}

	bool CircleCircleCollision(GameObjectEntity* & go,  GameObjectEntity*& go2) {
		f32 sqrDist = AEVec2SquareDistance(&go->position, &go2->position);
		f32 combinedRadii = go->scale.x + go2->scale.x;
		return sqrDist <= combinedRadii * combinedRadii;
	}

	/*bool CircleCircleCollision(const AEVec2& center_pos_1, const f32& radius_1, const AEVec2& center_pos_2, const f32& radius_2) {
		AEVec2 center_1 = { center_pos_1.x + radius_1, center_pos_1.y + radius_2 };
		AEVec2 center_2 = { center_pos_2.x + radius_1, center_pos_2.y + radius_2 };
		f32 sqrDisplacement = AEVec2SquareDistance(&center_1, &center_2);
		f32 combinedRadii = radius_1 + radius_2;
		return sqrDisplacement <= combinedRadii * combinedRadii;
	}*/

	/*bool AABB(const AEVec2& bottom_left_1, const f32& width_1, const f32& height_1, const AEVec2& bottom_left_2, const f32& width_2, const f32& height_2) {
		return !(bottom_left_1.x + width_1 < bottom_left_2.x || bottom_left_1.x > bottom_left_2.x + width_2 ||
			bottom_left_1.y + height_1 < bottom_left_2.y || bottom_left_1.y > bottom_left_2.y + height_2);
	}*/

	bool AABB(const GameObjectEntity* const& go, const GameObjectEntity* const& go2) {
		return !(go->position.x + go->scale.x * 0.5f < go2->position.x - go2->scale.x * 0.5f || go->position.x - go->scale.x * 0.5f > go2->position.x + go2->scale.x * 0.5f ||
			go->position.y + go->scale.y * 0.5f < go2->position.y - go2->scale.y * 0.5f || go->position.y - go->scale.y * 0.5f > go2->position.y + go2->scale.y * 0.5f);
	}

	static std::vector<AEVec2> getCorners(const GameObjectEntity* go) {
		std::vector<AEVec2> corners(4);
		float cosA = AECos(go->rotation);
		float sinA = AESin(go->rotation);

		// Local axes vectors
		AEVec2 dirX = { cosA, sinA };
		AEVec2 dirY = { -sinA, cosA };

		// Combine center with scaled axes
		corners[0] = { go->position.x + dirX.x * (go->scale.x * 0.5f) + dirY.x * (go->scale.y * 0.5f), go->position.y + dirX.y * (go->scale.x * 0.5f) + dirY.y * (go->scale.y * 0.5f) };
		corners[1] = { go->position.x - dirX.x * (go->scale.x * 0.5f) + dirY.x * (go->scale.y * 0.5f), go->position.y - dirX.y * (go->scale.x * 0.5f) + dirY.y * (go->scale.y * 0.5f) };
		corners[2] = { go->position.x - dirX.x * (go->scale.x * 0.5f) - dirY.x * (go->scale.y * 0.5f), go->position.y - dirX.y * (go->scale.x * 0.5f) - dirY.y * (go->scale.y * 0.5f) };
		corners[3] = { go->position.x + dirX.x * (go->scale.x * 0.5f) - dirY.x * (go->scale.y * 0.5f), go->position.y + dirX.y * (go->scale.x * 0.5f) - dirY.y * (go->scale.y * 0.5f) };

		return corners;
	}

	bool OBB(const GameObjectEntity* const& go, const GameObjectEntity* const& go2) {
		auto cornersA = getCorners(go);
		auto cornersB = getCorners(go2);

		// Axes to check: the normals of the sides of both rectangles
		// For a rectangle, we only need 2 axes per box (perpendicular sides)
		std::vector<AEVec2> axes = {
			{ AECos(go->rotation), AESin(go->rotation) },  // Box A Local X
			{ AESin(go->rotation), AECos(go->rotation) }, // Box A Local Y
			{ AECos(go2->rotation), AESin(go2->rotation) },  // Box B Local X
			{ AESin(go2->rotation), AECos(go2->rotation) }  // Box B Local Y
		};

		for (auto& axis : axes) {
			float minA = AEVec2DotProduct(&cornersA[0], &axis);
			float maxA = minA;
			for (size_t i = 1; i < 4; ++i) {
				float p = AEVec2DotProduct(&cornersA[i], &axis);
				if (p < minA) minA = p;
				if (p > maxA) maxA = p;
			}

			// Project corners of B
			float minB = AEVec2DotProduct(&cornersB[0], &axis);
			float maxB = minB;
			for (size_t i = 1; i < 4; ++i) {
				float p = AEVec2DotProduct(&cornersB[i], &axis);
				if (p < minB) minB = p;
				if (p > maxB) maxB = p;
			}

			// SAT Gap Check: If the "shadows" on this axis don't overlap, there's no collision
			if (maxA < minB || maxB < minA) {
				return false;
			}
		}

		return true; // Overlap on all axes means a collision
	}

	void SetDeltaTime(float dt) {
		deltaTime = dt;
	}

	void SnapVectorToAxis(AEVec2* result, AEVec2* vec2)
	{
		if (abs(vec2->x) > abs(vec2->y))
		{
			result->x = vec2->x / abs(vec2->x);
			result->y = 0.0;
		}
		else
		{
			result->x = 0.0;
			result->y = vec2->y / abs(vec2->y);
		}
	}
}