#pragma once
#ifndef __ASSETMANAGER_H
#define __ASSETMANAGER_H
#include "AEEngine.h"
#include "../Items/Image.hpp"
#include "../Items/SpriteSheet.hpp"
#include <map>
#include <string>

enum AssetType {
	TEXTURE = 0,
	SPRITE,
	AUDIO,
	FONT
};

struct Asset {
	AssetType type;
	Image* image;
	SpriteSheet* spritesheet;
	s8 font;
	AEAudio audio;
};

/*
* @brief Singleton class that handles creation and deletion of all assets (textures, fonts, audios, spritesheets)
*/
class AssetManager {
private:
	AssetManager();
	static AssetManager* instance;
	std::map<std::string, Asset*> asset_map;
public:
	~AssetManager();
	static AssetManager* GetInstance();
	static void Free();

	static Image* GetTexture(std::string file_name);
	static SpriteSheet* GetSpriteSheet(std::string file_name, int rows = 1, int cols = 1);
	static s8 GetFontId(std::string file_name);
	static AEAudio& GetAudio(std::string file_name);
};

#endif