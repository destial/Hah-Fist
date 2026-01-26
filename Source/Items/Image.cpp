#pragma comment(lib, "Ws2_32.lib")
#include "Image.hpp"
#include <fstream>
#include <stdint.h>
#include <string.h>

static uint32_t ntohl(uint32_t const net) {
	uint8_t data[4] = {};
	memcpy(&data, &net, sizeof(data));

	return ((uint32_t)data[3] << 0)
		| ((uint32_t)data[2] << 8)
		| ((uint32_t)data[1] << 16)
		| ((uint32_t)data[0] << 24);
}

Image::Image(const char* filename) : image(AEGfxTextureLoad(filename)), height(0), width(0) {
	if (image) {
		std::ifstream ifs(filename);
		ifs.seekg(16);
		ifs.read((char*)&width, 4);
		ifs.read((char*)&height, 4);
		width = static_cast<unsigned int>(ntohl(width));
		height = static_cast<unsigned int>(ntohl(height));
		ifs.close();
	}
}

Image::~Image() {
	if (image) {
		AEGfxTextureUnload(image);
		image = nullptr;
	}
}