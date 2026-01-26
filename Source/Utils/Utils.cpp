#include "Utils.hpp"
#include "AEMath.h"
#include "../Entities/GameObjectEntity.hpp"
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