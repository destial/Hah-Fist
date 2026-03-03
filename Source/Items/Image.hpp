/*!
* @file Image.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 26 January 2026
* @course CSD1451
* @brief Declaration file for an image wrapper
*/

#pragma once
#ifndef __IMAGE_H
#define __IMAGE_H
#include "AEEngine.h"

/*!
* @brief Wrapper class for AEGfxTexture to include
*		 the image's width and height
*/
class Image {
public:
	unsigned int width, height;
	AEGfxTexture* data;

	Image(const char* file); // Ctor
	~Image(); // Dtor
	Image& operator= (Image& copy); // Copy assignment
};

#endif