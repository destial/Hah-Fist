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

namespace Utils {
	static f32 world_width = 48.f;
	static f32 world_height = 27.f;

	static f32 deltaTime;

	unsigned int screen_width = 1600;
	unsigned int screen_height = 900;

	AEVec2 World_To_Screen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width),
			screen_height - y * (screen_height / world_height)
		};
	}

	AEVec2 Screen_To_World(f32 x, f32 y) {
		return {
			x * (world_width / screen_width),
			world_height - y * (world_height / screen_height)
		};
	}

	AEVec2 Game_To_Screen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width) - (static_cast<f32>(screen_width) * 0.5f),
			y * (screen_height / world_height) - (static_cast<f32>(screen_height) * 0.5f)
		};
	}

	AEVec2 Game_To_TextScreen(f32 x, f32 y) {
		return {
			((x / world_width) * 2.f) - 1.f,
			((y / world_height) * 2.f) - 1.f
		};
	}

	AEVec2 Scale_To_Screen(f32 x, f32 y) {
		return {
			x * (screen_width / world_width),
			y * (screen_height / world_height)
		};
	}

	AEVec2 Screen_To_Scale(f32 x, f32 y) {
		return {
			x / (screen_width / world_width),
			y / (screen_height / world_height)
		};
	}

	const AEVec2 Get_Mouse_World(bool cam) {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };

		f32 cam_x, cam_y;
		AEGfxGetCamPosition(&cam_x, &cam_y);
		return Utils::Screen_To_World(mouse.x + (cam ? cam_x : 0.f), mouse.y + (cam ? -cam_y : 0.f));
	}

	u32 PackColor(int red, int green, int blue, int alpha) {
		u32 color = (alpha << 24) + (red << 16) + (green << 8) + (blue);
		return color;
	}

	u32 PackColor(Color const& color) {
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

	const f32 GetWorldWidth(void) {
		return world_width;
	}

	const f32 GetWorldHeight(void) {
		return world_height;
	}

	const f32 GetDeltaTime(void) {
		return deltaTime;
	}

	bool CircleCircleCollision(BaseEntity*& go, BaseEntity*& go2) {
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

	bool AABB(const BaseEntity* const& go, const BaseEntity* const& go2) {
		return !(go->position.x + go->scale.x * 0.5f < go2->position.x - go2->scale.x * 0.5f || go->position.x - go->scale.x * 0.5f > go2->position.x + go2->scale.x * 0.5f ||
			go->position.y + go->scale.y * 0.5f < go2->position.y - go2->scale.y * 0.5f || go->position.y - go->scale.y * 0.5f > go2->position.y + go2->scale.y * 0.5f);
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
		corners[0] = { go->position.x + dirX.x * (go->scale.x * 0.5f) + dirY.x * (go->scale.y * 0.5f), go->position.y + dirX.y * (go->scale.x * 0.5f) + dirY.y * (go->scale.y * 0.5f) };

		// bottom right
		corners[1] = { go->position.x - dirX.x * (go->scale.x * 0.5f) + dirY.x * (go->scale.y * 0.5f), go->position.y - dirX.y * (go->scale.x * 0.5f) + dirY.y * (go->scale.y * 0.5f) };

		// bottom left
		corners[2] = { go->position.x - dirX.x * (go->scale.x * 0.5f) - dirY.x * (go->scale.y * 0.5f), go->position.y - dirX.y * (go->scale.x * 0.5f) - dirY.y * (go->scale.y * 0.5f) };

		// top left
		corners[3] = { go->position.x + dirX.x * (go->scale.x * 0.5f) - dirY.x * (go->scale.y * 0.5f), go->position.y + dirX.y * (go->scale.x * 0.5f) - dirY.y * (go->scale.y * 0.5f) };

		return corners;
	}

	bool OBB(const BaseEntity* const& go, const BaseEntity* const& go2) {
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

			// SAT Gap Check: If the "shadows" on this axis don't overlap, there's no collision
			if (maxA < minB || maxB < minA) {
				return false;
			}
		}

		return true; // Overlap on all axes means a collision
	}

	bool OBBPoint(const BaseEntity* const& go, AEVec2 const& pos) {
		AEVec2 local_pos = pos - go->position;
		AEVec2Rotate(&local_pos, &local_pos, go->rotation);

		return (local_pos.x <= go->scale.x * 0.5f && local_pos.x >= -go->scale.x * 0.5f &&
			local_pos.y <= go->scale.y * 0.5f && local_pos.y >= -go->scale.y * 0.5f);
	}

	void SetDeltaTime(f32 dt) {
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

	AEMtx33 GetTransformMatrix(AEVec2 const& pos, AEVec2 const& sca, f32 rot) {
		AEMtx33 scale{ 1.f };
		AEVec2 s_scale = Utils::Scale_To_Screen(sca.x, sca.y);
		AEMtx33Identity(&scale);
		AEMtx33Scale(&scale, s_scale.x, s_scale.y);
		AEMtx33 rotate = { 0 };
		AEMtx33Identity(&rotate);
		AEMtx33Rot(&rotate, rot);
		AEVec2 screenPos = Utils::Game_To_Screen(pos.x, pos.y);
		AEMtx33 translate = { 0 };
		AEMtx33Identity(&translate);
		AEMtx33Trans(&translate, screenPos.x, screenPos.y);
		return translate * rotate * scale;
	}

	f32 Math_Lerp(f32 start, f32 end, f32 delta) {
		return start + min_max(delta, 0.f, 1.f) * (end - start);
	}

	int Math_Lerp(int start, int end, f32 delta) {
		return static_cast<int>(start + min_max(delta, 0.f, 1.f) * (end - start));
	}

	f32 Math_CLerp(f32 start, f32 end, f32 delta) {
		delta = min_max(delta, 0.f, 1.f);
		f32 p = 1.f - ((AECos(PI * delta) + 1.f) * 0.5f);
		return Math_Lerp(start, end, p);
	}

	int Math_CLerp(int start, int end, f32 delta) {
		delta = min_max(delta, 0.f, 1.f);
		f32 p = 1.f - ((AECos(PI * delta) + 1.f) * 0.5f);
		return Math_Lerp(start, end, p);
	}
}