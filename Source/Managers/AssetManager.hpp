/*!
* @file AssetManager.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 19 January 2026
* @course CSD1451
* @brief Declaration file for an asset manager that will handle loading and freeing of assets
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

/*!
* @brief Declare wrapper class for the different types of Asset
*/
struct Asset {
	AssetType type; // enum type for asset
	union { // union for space saving since at most, each value is 8 bytes
		Image* image;
		s8 font;
		AEAudio* audio;
		SpriteSheet* spritesheet;
	};
};

/*!
* @brief Singleton class that handles creation and deletion of all assets (textures, fonts, audios, spritesheets)
*/
class AssetManager : public Singleton<AssetManager> {
private:
	std::map<std::string, Asset*> asset_map; // store the assets
public:
	AssetManager(); // Ctor
	~AssetManager(); // Dtor

	/*!
	* @brief Get the Image texture at this file location
	* @param file_name - The file location
	* @return The loaded image pointer, or null if not found
	*/
	static Image* GetTexture(std::string file_name);

	/*!
	* @brief Get the SpriteSheet at this file location
	* @param file_name - The file location
	* @param rows - How many rows in this spritesheet
	* @param cols - How many columns in this spritesheet
	* @return The loaded spritesheet pointer, or null if not found
	*/
	static SpriteSheet* GetSpriteSheet(std::string file_name, int rows = 1, int cols = 1);

	/*!
	* @brief Get the Font id at this file location
	* @param file_name - The file location
	* @return The loaded font id, or -1 if not found
	*/
	static s8 GetFontId(std::string file_name);

	/*!
	* @brief Get the Audio source at this file location
	* @param file_name - The file location
	* @return The loaded audio pointer, or null if not found
	*/
	static AEAudio* GetAudio(std::string file_name);
};

#endif