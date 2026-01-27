#include "Utils.hpp"
#include "AEMath.h"
#include "../Entities/GameObjectEntity.hpp"
#include "AEOverload.hpp"
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

	static bool isGap(AEVec2& L, OBBStruct& a, OBBStruct& b) {
		// Vector between centers
		AEVec2 T = a.center - b.center;

		// Project the distance between centers onto the axis L
		float distance = std::abs(AEVec2DotProduct(&T, &L));

		// Project the "radius" of box A onto axis L
		float radiusA = std::abs(AEVec2DotProduct(&a.axes[0], &L) * a.halfWidths[0]) +
			std::abs(AEVec2DotProduct(&a.axes[1], &L) * a.halfWidths[1]);

		// Project the "radius" of box B onto axis L
		float radiusB = std::abs(AEVec2DotProduct(&b.axes[0], &L) * b.halfWidths[0]) +
			std::abs(AEVec2DotProduct(&b.axes[1], &L) * b.halfWidths[1]);

		// If distance is greater than the sum of the radii, there is a gap
		return distance > (radiusA + radiusB);
	}

	bool OBB(const GameObjectEntity* const& go, const GameObjectEntity* const& go2) {
		// TODO: Oriented bounding box collision (still trying to understand SAT)
#if 1
		return AABB(go, go2);
#endif
		AEVec2 top_a = { go->scale.x, 0.f };
		AEVec2 left_a = { 0.f, go->scale.y };
		AEVec2Rotate(&top_a, &top_a, go->rotation);
		AEVec2Rotate(&left_a, &left_a, go->rotation);

		AEVec2 top_b = { go2->scale.x, 0.f };
		AEVec2 left_b = { 0.f, go2->scale.y };
		AEVec2Rotate(&top_b, &top_b, go2->rotation);
		AEVec2Rotate(&left_b, &left_b, go2->rotation);

		OBBStruct a{ go->position, {top_a, left_a}, {go->scale.x * 0.5f, go->scale.y * 0.5f} };
		OBBStruct b{ go2->position, {top_b, left_b}, {go2->scale.x * 0.5f, go2->scale.y * 0.5f} };

		if (isGap(a.axes[0], a, b)) return false;
		if (isGap(a.axes[1], a, b)) return false;
		if (isGap(b.axes[0], a, b)) return false;
		if (isGap(b.axes[1], a, b)) return false;

		// No gap found on any axis = collision
		return true;
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