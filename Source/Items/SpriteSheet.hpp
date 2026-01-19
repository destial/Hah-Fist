#pragma once
#ifndef __SPRITEANIMATION_H
#define __SPRITEANIMATION_H
#include "AEEngine.h"

class SpriteSheet {
protected:
	int rows, columns;
	AEGfxTexture* texture;
	AEGfxVertexList* mesh;
public:
	SpriteSheet(const char* file, int rows, int columns);
	~SpriteSheet();

	void Render(AEMtx33& t, int row, int column);
};

#endif