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

#define ASSET_PLAYER_SPRITE "Assets/Entities/player.png"
#define ASSET_TROOPER_SPRITE "Assets/Entities/test_troop.png"
#define ASSET_TROOPER_SPRITE_ROWS 3
#define ASSET_TROOPER_SPRITE_COLUMNS 3
#define ASSET_TROOPER_SPRITE_SCALE 5.f

#define ASSET_BASEENEMY_SPRITE "Assets/Entities/test_enemy.png"
#define ASSET_BASEENEMY_SPRITE_ROWS 3
#define ASSET_BASEENEMY_SPRITE_COLUMNS 3
#define ASSET_BASEENEMY_SPRITE_SCALE 5.f

#define ASSET_SLIMETROOP_SPRITE "Assets/Entities/slime.png"
#define ASSET_SLIMETROOP_SPRITE_ROWS 1
#define ASSET_SLIMETROOP_SPRITE_COLUMNS 30
#define ASSET_SLIMETROOP_SPRITE_SCALE 5.f

#define ASSET_PROJECTILE_ENEMY_SPRITE "Assets/Entities/Projectile_Enemy.png"
#define ASSET_PROJECTILE_ENEMY_SPRITE_ROWS 7
#define ASSET_PROJECTILE_ENEMY_SPRITE_COLUMNS 8
#define ASSET_PROJECTILE_ENEMY_SPRITE_SCALE 5.f

#define ASSET_SPIDER_SPRITE "Assets/Entities/spider_enemy.png"
#define ASSET_SPIDER_SPRITE_ROWS 4
#define ASSET_SPIDER_SPRITE_COLUMNS 6
#define ASSET_SPIDER_SPRITE_SCALE 5.f

#define ASSET_TITLE_SPRITE "Assets/Icons/title.png"
#define ASSET_SPARKLE_SPRITE "Assets/Icons/sparkle_sheet.png"
#define ASSET_STAR_SPRITE "Assets/Icons/star.png"
#define ASSET_CREDITS_SPRITE "Assets/Icons/credits_sprite.png"
#define ASSET_TITAN_SPRITE "Assets/Entities/titan.png"
#define ASSET_TITAN_SPRITE_ROWS 3
#define ASSET_TITAN_SPRITE_COLUMNS 20
#define ASSET_TITAN_SPRITE_SCALE 5.f

#define ASSET_PAYLOAD_SPRITE "Assets/Entities/payload.png"
#define ASSET_PAYLOAD_SPRITE_ROWS 3
#define ASSET_PAYLOAD_SPRITE_COLUMNS 18
#define ASSET_PAYLOAD_SPRITE_SCALE 5.f

#define ASSET_COIN_SPRITE "Assets/Entities/coin.png"
#define ASSET_COIN_SPRITE_ROWS 1
#define ASSET_COIN_SPRITE_COLUMNS 20
#define ASSET_COIN_SPRITE_SCALE 2.f
#define ASSET_COIN_PICKUP_AUDIO "Assets/Audio/coin_pickup.wav"

#define ASSET_BREAKABLE_CRATE_IMAGE "Assets/Entities/crate.png"
#define ASSET_BREAKING_FRAME_SPRITE "Assets/Entities/breaking_frames.png"

#define ASSET_PROJECTILE_IMAGE "Assets/Entities/projectile.png"
#define ASSET_SPLASHSCREEN_IMAGE "Assets/Icons/splash_screen.png"
#define ASSET_TURBOFIST_IMAGE "Assets/Entities/fist1.png"
#define ASSET_GRAPPLEFIST_IMAGE "Assets/Entities/fist2.png"
#define ASSET_FINGERGUN_IMAGE "Assets/Entities/fist3.png"
#define ASSET_SMALLBUTTON_IMAGE "Assets/Icons/small_button_grey.png"
#define ASSET_BACKGROUND_IMAGE "Assets/Icons/background.png"
#define ASSET_HUD_IMAGE "Assets/Icons/panel_brown.png"

#define ASSET_DEFAULT_FONT "Assets/Fonts/Jersey25-Regular.ttf"
#define ASSET_DEBUG_FONT "Assets/Fonts/buggy-font.ttf"

#define ASSET_MAINMENUBGM_AUDIO "Assets/Audio/bgm.mp3"
#define ASSET_TURBOFIST_AUDIO "Assets/Audio/turbofist.ogg"
#define ASSET_ENEMYHURT_AUDIO "Assets/Audio/enemy_hurt.mp3"
#define ASSET_SPIDERHURT_AUDIO "Assets/Audio/spider_hurt.ogg"
#define ASSET_PLAYERHURT_AUDIO "Assets/Audio/player_hurt.ogg"

#define ASSET_BOSS_SPAWN_AUDIO "Assets/Audio/boss_trigger_close.wav"

//Gameplay Macros
#define PLAYER_INVULNERABILITY_ON_HIT 0.75f
#define PLAYER_CONTROL_LOCK_AFTER_HIT 0.5f

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
		AEAudio audio;
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
	* @return The loaded audio, or null if not found
	*/
	static AEAudio GetAudio(std::string file_name);
};

#endif