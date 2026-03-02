/*!
@author Rance Andres (andresrancerowell.g@digipen.edu)
@date 2 March 2026
@course CSD1451
@brief Declaration file for an asset manager that will handle loading and freeing of assets
*/

#pragma once
#ifndef __ASSETMANAGER_H
#define __ASSETMANAGER_H
#include "AEEngine.h"
#include "../Items/Image.hpp"
#include "../Items/SpriteSheet.hpp"
#include "../Utils/Singleton.hpp"
#include <map>
#include <string>

/*!
* @brief Declare asset type
*/
enum class AssetType {
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
class AssetManager : public Singleton<AssetManager> {
private:
	std::map<std::string, Asset*> asset_map;
public:
	AssetManager();
	~AssetManager();

	static Image* GetTexture(std::string file_name);
	static SpriteSheet* GetSpriteSheet(std::string file_name, int rows = 1, int cols = 1);
	static s8 GetFontId(std::string file_name);
	static AEAudio& GetAudio(std::string file_name);
};

#endif