#pragma once
#ifndef __ASSETMANAGER_H
#define __ASSETMANAGER_H
#include "AEEngine.h"
#include <map>
#include <string>

enum AssetType {
	TEXTURE = 0,
	AUDIO,
	FONT
};

struct Asset {
	AssetType type;
	union {
		AEGfxTexture* texture;
		s8 font;
		AEAudio audio;
		void* val;
	};
};

class AssetManager {
private:
	AssetManager();
	static AssetManager* instance;
	std::map<std::string, Asset*> asset_map;
public:
	~AssetManager();
	static AssetManager* GetInstance();
	static void Free();

	static AEGfxTexture* GetTexture(std::string file_name);
	static s8 GetFontId(std::string file_name);
	static AEAudio& GetAudio(std::string file_name);
};

#endif