#include "Utils.hpp"
#include "AEMath.h"
namespace Utils {
	static float world_width = 48.f;
	static float world_height = 27.f;

	static float deltaTime;

	AEVec2 World_To_Screen(float x, float y) {
		return {
			x * (1600.f / world_width),
			900.f - y * (900.f / world_height)
		};
	}

	AEVec2 Screen_To_World(float x, float y) {
		return {
			x * (world_width / 1600.f),
			world_height - y * (world_height / 900.f)
		};
	}

	AEVec2 Game_To_Screen(float x, float y) {
		return {
			x * (1600.f / world_width) - 800.f,
			y * (900.f / world_height) - 450.f
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
			x * (1600.f / world_width),
			y * (900.f / world_height)
		};
	}

	const AEVec2 Get_Mouse_World() {
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
		return Utils::Screen_To_World(mouse.x, mouse.y);
	}

	u32 ConvertToColor(int red, int green, int blue, int alpha) {
		u32 color = (alpha << 24) + (red << 16) + (green << 8) + (blue);
		return color;
	}

	Color ConvertFromColor(u32 color) {
		Color c{ 0, 0, 0, 0 };
		c.a = ((color >> 24) & 255);
		c.r = ((color >> 16) & 255);
		c.g = ((color >> 8) & 255);
		c.b = (color & 255);
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

	bool CircleCircleCollision(const AEVec2& center_pos_1, const f32& radius_1, const AEVec2& center_pos_2, const f32& radius_2) {
		AEVec2 center_1 = { center_pos_1.x + radius_1, center_pos_1.y + radius_2 };
		AEVec2 center_2 = { center_pos_2.x + radius_1, center_pos_2.y + radius_2 };
		f32 sqrDisplacement = AEVec2SquareDistance(&center_1, &center_2);
		f32 combinedRadii = radius_1 + radius_2;
		return sqrDisplacement <= combinedRadii * combinedRadii;
	}

	bool AABB(const AEVec2& bottom_left_1, const f32& width_1, const f32& height_1, const AEVec2& bottom_left_2, const f32& width_2, const f32& height_2) {
		return !(bottom_left_1.x + width_1 < bottom_left_2.x || bottom_left_1.x > bottom_left_2.x + width_2 ||
			bottom_left_1.y + height_1 < bottom_left_2.y || bottom_left_1.y > bottom_left_2.y + height_2);
	}

	void SetDeltaTime(float dt) {
		deltaTime = dt;
	}

	

}