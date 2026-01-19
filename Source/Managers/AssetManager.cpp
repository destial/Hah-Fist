#include "AssetManager.hpp"

AssetManager* AssetManager::instance = nullptr;

AssetManager::AssetManager() {
}

AssetManager::~AssetManager() {
	for (auto& entry : asset_map) {
		if (entry.second->type == TEXTURE && entry.second->texture) {
			AEGfxTextureUnload(entry.second->texture);
		}
		else if (entry.second->type == FONT && entry.second->font) {
			AEGfxDestroyFont(entry.second->font);
		}
		else if (entry.second->type == AUDIO) {
			AEAudioUnloadAudio(entry.second->audio);
		}
		else if (entry.second->type == SPRITE) {
			delete entry.second->spritesheet;
		}

		delete entry.second;
	}
	asset_map.clear();
}

void AssetManager::Free() {
	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

AEGfxTexture* AssetManager::GetTexture(std::string file_name) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->texture;
	}
	instance->asset_map[file_name] = new Asset();
	instance->asset_map[file_name]->texture = AEGfxTextureLoad(file_name.c_str());
	instance->asset_map[file_name]->type = TEXTURE;
	return instance->asset_map[file_name]->texture;
}

SpriteSheet* AssetManager::GetSpriteSheet(std::string file_name, int rows, int cols) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->spritesheet;
	}
	instance->asset_map[file_name] = new Asset();
	instance->asset_map[file_name]->spritesheet = new SpriteSheet(file_name.c_str(), rows, cols);
	instance->asset_map[file_name]->type = SPRITE;
	return instance->asset_map[file_name]->spritesheet;
}

s8 AssetManager::GetFontId(std::string file_name) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->font;
	}
	instance->asset_map[file_name] = new Asset();
	instance->asset_map[file_name]->font = AEGfxCreateFont(file_name.c_str(), 72);
	instance->asset_map[file_name]->type = FONT;
	return instance->asset_map[file_name]->font;
}

AEAudio& AssetManager::GetAudio(std::string file_name) {
	GetInstance();
	if (instance->asset_map.count(file_name)) {
		return instance->asset_map[file_name]->audio;
	}
	instance->asset_map[file_name] = new Asset();
	instance->asset_map[file_name]->audio = AEAudioLoadSound(file_name.c_str());
	instance->asset_map[file_name]->type = AUDIO;
	return instance->asset_map[file_name]->audio;
}

AssetManager* AssetManager::GetInstance() {
	if (!instance) {
		instance = new AssetManager();
	}
	return instance;
}