#pragma once
#ifndef __IMAGE_H
#define __IMAGE_H
#include "AEEngine.h"

class Image {
public:
	unsigned int width, height;
	AEGfxTexture* image;

	Image(const char* file);
	~Image();
};

#endif