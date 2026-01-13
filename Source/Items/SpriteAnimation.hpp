#pragma once
#ifndef __SPRITEANIMATION_H
#define __SPRITEANIMATION_H
#include "AEEngine.h"

class SpriteAnimation {
private:
	int rows, columns;
	AEGfxTexture* texture;
	AEGfxVertexList* mesh;
public:
	SpriteAnimation(const char* file, int rows, int columns);
	~SpriteAnimation();

	void Render(AEMtx33& t, int row, int column);
};

#endif