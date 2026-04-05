/*!
* @file Debug.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 11 March 2026
* @course CSD1451
* @brief Declaration file for debugging unto the screen
*/

#pragma once
#ifndef __DEBUG_H
#define __DEBUG_H
#include "AEEngine.h"
#include <string>
#include "../Utils/Utils.hpp"

namespace DebugUtils {
	/*!
	* @brief If the debug tool is currently rendering to the screen
	* @return The render state
	*/
	bool IsRendering();

	/*!
	* @brief Toggle the debug tool render state
	* @param state - The render state
	*/
	void ToggleRender(bool state);

	/*!
	* @brief Render a debug text unto the screen
	* @param world_pos - The world coordinates
	* @param text - The text to display
	* @param color - The text color
	* @param size - The text size
	* @param cam - If to take account camera position
	*/
	void RenderText(AEVec2 world_pos, std::string text, Color color = { 255, 0, 255, 0 }, f32 size = 2.5f, bool cam = false);

	/*!
	* @brief Render a debug text unto the screen
	* @param world_pos - The world coordinates
	* @param text - The text to display
	* @param cam - If to take account camera position
	*/
	void RenderText(AEVec2 world_pos, std::string text, bool cam);

	/*!
	* @brief Render a debug point unto the screen
	* @param world_pos - The world coordinates
	* @param color - The point color
	*/
	void RenderPoint(AEVec2 world_pos, Color color = { 255, 0, 255, 0 });

	/*!
	* @brief Render a debug line unto the screen
	* @param point_a - The start of the line
	* @param point_b - The end of the line
	* @param color - The line color
	*/
	void RenderLine(AEVec2 point_a, AEVec2 point_b, Color color = { 255, 0, 255, 0 });
}

#endif