#pragma once
#ifndef __SPRITEANIMATION_H
#define __SPRITEANIMATION_H
#include "AEEngine.h"
#include "Image.hpp"

/*
* @brief Represents a image formatted as a spritesheet
*/
class SpriteSheet {
protected:
	int rows, columns;
	AEGfxVertexList* mesh;
public:
	Image* image;
	SpriteSheet(const char* file, int rows, int columns);
	~SpriteSheet();

	void Render(AEMtx33& t, int row, int column);
};

#endif