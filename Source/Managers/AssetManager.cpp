/*!
* @file AssetManager.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 19 January 2026
* @course CSD1451
* @brief Definition file for an asset manager that will handle loading and freeing of assets
*/

#include "AssetManager.hpp"

AssetManager::AssetManager() {} // Ctor

AssetManager::~AssetManager() { // Dtor
	for (auto& entry : asset_map) {
		if (entry.second->type == AssetType::TEXTURE && entry.second->image) {
			delete entry.second->image;
		}
		else if (entry.second->type == AssetType::FONT && entry.second->font != -1) {
			AEGfxDestroyFont(entry.second->font);
		}
		else if (entry.second->type == AssetType::AUDIO) {
			AEAudioUnloadAudio(entry.second->audio);
		}
		else if (entry.second->type == AssetType::SPRITE) {
			delete entry.second->spritesheet;
		}

		delete entry.second;
	}
	asset_map.clear();
}

/*!
* @brief Get the Image texture at this file location
* @param file_name - The file location
* @return The loaded image pointer, or null if not found
*/
Image* AssetManager::GetTexture(std::string file_name) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->image;
	}
	instance->asset_map[file_name] = new Asset;
	instance->asset_map[file_name]->image = new Image{ file_name.c_str() };
	instance->asset_map[file_name]->type = AssetType::TEXTURE;
	return instance->asset_map[file_name]->image;
}

/*!
* @brief Get the SpriteSheet at this file location
* @param file_name - The file location
* @param rows - How many rows in this spritesheet
* @param cols - How many columns in this spritesheet
* @return The loaded spritesheet pointer, or null if not found
*/
SpriteSheet* AssetManager::GetSpriteSheet(std::string file_name, int rows, int cols) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->spritesheet;
	}
	instance->asset_map[file_name] = new Asset;
	instance->asset_map[file_name]->spritesheet = new SpriteSheet{ file_name.c_str(), rows, cols };
	instance->asset_map[file_name]->type = AssetType::SPRITE;
	return instance->asset_map[file_name]->spritesheet;
}

/*!
* @brief Get the Font id at this file location
* @param file_name - The file location
* @return The loaded font id, or -1 if not found
*/
s8 AssetManager::GetFontId(std::string file_name) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->font;
	}
	instance->asset_map[file_name] = new Asset;
	instance->asset_map[file_name]->font = AEGfxCreateFont(file_name.c_str(), 50);
	instance->asset_map[file_name]->type = AssetType::FONT;
	return instance->asset_map[file_name]->font;
}

/*!
* @brief Get the Audio source at this file location
* @param file_name - The file location
* @return The loaded audio, or null if not found
*/
AEAudio AssetManager::GetAudio(std::string file_name) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->audio;
	}
	AEAudio audio = AEAudioLoadSound(file_name.c_str());
	if (AEAudioIsValidAudio(audio)) {
		instance->asset_map[file_name] = new Asset;
		instance->asset_map[file_name]->audio = audio;
		instance->asset_map[file_name]->type = AssetType::AUDIO;
		return instance->asset_map[file_name]->audio;
	}
	return AEAudio();
}