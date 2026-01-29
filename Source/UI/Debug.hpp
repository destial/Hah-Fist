#pragma once
#ifndef __DEBUG_H
#define __DEBUG_H
#include "AEEngine.h"
#include "../Utils/Utils.hpp"

namespace DebugUtils {
	bool IsRendering();
	void ToggleRender(bool state);
	void RenderPoint(AEVec2 world_pos, Color color = {255, 0, 0, 255});
	void RenderLine(AEVec2 point_a, AEVec2 point_b, Color color = { 255, 0, 0, 255 });
}

#endif