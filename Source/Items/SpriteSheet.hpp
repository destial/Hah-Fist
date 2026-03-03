/*!
* @file SpriteSheet.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Declaration file for sprite sheets used for animation
*/

#pragma once
#ifndef __SPRITEANIMATION_H
#define __SPRITEANIMATION_H
#include "AEEngine.h"
#include "Image.hpp"
#include "../Utils/Utils.hpp"

/*
* @brief Represents a data formatted as a spritesheet
*/
class SpriteSheet {
protected:
	int rows, columns; // Total rows and columns for the spritesheet
	AEGfxVertexList* mesh; // The wrapper for the mesh data after calculating UV values
public:
	Image* image; // The image wrapper
	SpriteSheet(const char* file, int rows, int columns); // Ctor
	~SpriteSheet(); // Dtor

	/*!
	* @brief Render the spritesheet
	* @param t - The transformation matrix to apply
	* @param color - The color to multiply with
	* @param row - The spritesheet row
	* @pararm column - The spritesheet column
	*/
	void Render(AEMtx33& t, Color color, int row, int column);
};

#endif