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
* @brief the image's width and height
*/
class Image {
public:
	unsigned int width, height; // Image width and height in pixels
	AEGfxTexture* data; // AE pointer wrapper

	Image(const char* file); // Ctor
	Image(Image const&) = delete; // Remove copy ctor
	Image& operator=(Image const&) = delete; // Remove copy assignment
	~Image(); // Dtor
};

#endif