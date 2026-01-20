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

	void SetDeltaTime(float dt) {
		deltaTime = dt;
	}
}