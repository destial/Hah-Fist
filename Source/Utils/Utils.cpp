#include "Utils.hpp"

AEVec2 World_To_Screen(float x, float y, float world_width, float world_height) {
	return {
		x * (1600.f / world_width),
		y * (900.f / world_height)
	};
}

AEVec2 Screen_To_World(float x, float y, float world_width, float world_height) {
	return {
		x * (world_width / 1600.f),
		y * (world_height / 900.f)
	};
}