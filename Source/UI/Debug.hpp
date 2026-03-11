#pragma once
#ifndef __DEBUG_H
#define __DEBUG_H
#include "AEEngine.h"
#include <string>
#include "../Utils/Utils.hpp"

namespace DebugUtils {
	bool IsRendering();
	void ToggleRender(bool state);
	void RenderText(AEVec2 world_pos, std::string text, Color color = { 255, 0, 255, 0 }, f32 size = 2.5f, bool cam = false);
	void RenderText(AEVec2 world_pos, std::string text, bool cam);
	void RenderPoint(AEVec2 world_pos, Color color = { 255, 0, 255, 0 });
	void RenderLine(AEVec2 point_a, AEVec2 point_b, Color color = { 255, 0, 255, 0 });
}

#endif