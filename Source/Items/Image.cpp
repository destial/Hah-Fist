/*!
* @file Image.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 26 January 2026
* @course CSD1451
* @brief Definition file for an image wrapper
*/

#pragma comment(lib, "Ws2_32.lib")
#include "Image.hpp"
#include <algorithm>
#include <fstream>
#include <stdint.h>
#include <string.h>

/*!
* @brief Convert from big endian to little endian
* Source: https://codereview.stackexchange.com/questions/149717/implementation-of-c-standard-library-function-ntohl
*/
static uint32_t ntohl(uint32_t const net) {
	uint8_t data[4] = {};
	memcpy(&data, &net, sizeof(data));

	return ((uint32_t)data[3] << 0)
		| ((uint32_t)data[2] << 8)
		| ((uint32_t)data[1] << 16)
		| ((uint32_t)data[0] << 24);
}

Image::Image(const char* filename)
: data{ AEGfxTextureLoad(filename) }, height{0}, width{0} {
	if (data) {
		// Read the image file and get the width and height data
		std::ifstream ifs{ filename };
		ifs.seekg(16);
		ifs.read((char*)&width, 4);
		ifs.read((char*)&height, 4);
		width = static_cast<unsigned int>(ntohl(width));
		height = static_cast<unsigned int>(ntohl(height));
		ifs.close();
	}
}

Image::~Image() {
	if (data) {
		AEGfxTextureUnload(data);
		data = nullptr;
	}
}
