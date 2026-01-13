#include "Utils.hpp"

static float world_width = 16.f;
static float world_height = 9.f;

AEVec2 World_To_Screen(float x, float y) {
	return {
		x * (1600.f / world_width),
		900.f - y * (900.f / world_height)
	};
}

AEVec2 Game_To_Screen(float x, float y) {
	return {
		x * (1600.f / world_width) - 800.f,
		y * (900.f / world_height) - 450.f
	};
}

AEVec2 Screen_To_World(float x, float y) {
	return {
		x * (world_width / 1600.f),
		world_height - y * (world_height / 900.f)
	};
}