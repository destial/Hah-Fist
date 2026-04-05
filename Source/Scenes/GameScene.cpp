/*!
* @file GameScene.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 7 January 2026
* @course CSD1451
* @brief Definition file for a base scene that will be inherited for all scenes
*/

#include "GameScene.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SerializationManager.hpp"
#include "../Managers/LevelManager.hpp"
#include "../Entities/GameObjectEntity.hpp"
#include "../Entities/StaticEntity.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/Enemies/TitanEntity.hpp"
#include "../Entities/Enemies/PayloadEntity.hpp"
#include "../Entities/Enemies/IronsideEntity.hpp"
#include "../Entities/Weapons/TurboFistEntity.hpp"
#include "../Entities/Weapons/GrappleFistEntity.hpp"
#include "../Entities/Weapons/FingerGunEntity.hpp"
#include "../Entities/DropEntities/CoinEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/Constant.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/BarUI.hpp"
#include "../UI/ImageUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../UI/Debug.hpp"
#include "AEMath.h"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

constexpr int SIZE_TO_RESERVE{ 50 }; // Reserves this amt of space for vector arr
constexpr float VOL_SCALE_X = 12.5f; // Volume slider scale
constexpr float VOL_SCALE_Y = 4.f; // Volume slider scale

std::vector<BaseEntity*> GameScene::staticEntities;

/*!
* @brief Generic function to create a display for real-time interactive hotkeys
* @param pos - The position on the screen
* @param str - The text to display
* @param listener - The input to listen to
* @return The ButtonUI instance created
*/
static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, std::string str, u8 listener) {
	ButtonUI* b = new ButtonUI{ pos };
	b->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	b->SetInteractive(false);
	b->color = { 255, 255, 255, 255 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->overlay_color = b->color;
	b->text = str;
	b->text_size = 7.5f;
	b->font = AssetManager::GetFontId(ASSET_DEFAULT_FONT);

	// Always set color multiplier to white when not hovered
	b->AddPreUpdateListener(b, [b](const f32&) {
		b->color = { 255, 255, 255, 255 };
	});
	b->AddUpdateListener(b, [b, listener](const f32&) {
		if (AEInputCheckCurr(listener)) {
			b->color = { 255, 128, 128, 128 };
		}
	});
	return b;
}

/*!
* @brief Generic function to create a display for real-time interactive hotkeys
* @param pos - The position on the screen
* @param str - The character to display
* @param listener - The input to listen to
* @return The ButtonUI instance created
*/
static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, char ch, u8 listener) {
	return CreateHotKeyDisplay(pos, std::string{ ch }, listener);
}

GameScene::GameScene() : BaseScene(), game_timer{ 0 }, game_state{ GameState::INIT } {} // Empty ctor body

GameScene::~GameScene() {} // Empty dtor

/*!
* @brief Inherited: Initialize the scene with starting points, entities, level, etc.
*/
void GameScene::Init() {
	staticEntities.reserve(SIZE_TO_RESERVE); // Reserves StaticEntities capacity for vector arr
	camManager->Init();
	game_timer = 0.f;

	std::vector<BaseUI*> pause_menu;

	// Initialize win screen background
	ImageUI* pause = new ImageUI{ ASSET_HUD_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f} };
	pause->scale = { Utils::GetWorldWidth() * 0.3f, Utils::GetWorldHeight() * 0.82f };
	AddEntityToScene(pause);
	pause_menu.push_back(pause);

	// Initialize win screen text
	ButtonUI* toptext = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.7f } };
	toptext->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	toptext->text_size = 10.f;
	toptext->color.a = 0;
	toptext->text = "Game Paused!    ";
	toptext->SetInteractive(false);
	AddEntityToScene(toptext);
	pause_menu.push_back(toptext);

	// Initialize next level button
	ButtonUI* advance = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.6f } };
	advance->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	advance->text_size = 10.f;
	advance->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	advance->text = "Continue    ";

	AddEntityToScene(advance);
	pause_menu.push_back(advance);

	// Initialize the return to main menu button
	ButtonUI* back = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.4f } };
	back->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	back->text_size = 10.f;
	back->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	back->text = "Back to Menu   ";
	AddEntityToScene(back);
	pause_menu.push_back(back);

	// Listen to button click to go back to main menu scene
	back->AddClickListener([this](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
		}
	});

	// Initalize master volume slider
	BarUI* music_vol = new BarUI({ Utils::GetWorldWidth() * 0.5f, 6.f });
	music_vol->text = "";
	music_vol->SetValue(Game::MusicVol());
	music_vol->scale = { VOL_SCALE_X * 0.75f, VOL_SCALE_Y * 0.35f };
	music_vol->text_size = 7.5f;
	music_vol->text_alignment = BaseUI::TextAlignment::LEFT_CORNER;
	music_vol->AddUpdateListener(this, [music_vol](const f32&) {
		// Update group volume based on slider value when interacted with
		AEAudioSetGroupVolume(Game::GetMusicGroup(), AEClamp(music_vol->GetValue(), 0.f, 1.f));
		Game::MusicVol() = music_vol->GetValue();

		// Update text on slider
		char vol[64];
		sprintf_s(vol, 64, "Master Volume: %0.2f", music_vol->GetValue() * 100.f);
		music_vol->text = vol;

		// Change slider color based on interaction
		if (music_vol->IsDragging()) {
			music_vol->overlay_color = { 255, 64, 128, 64 };
		}
		else {
			music_vol->overlay_color = { 255, 64, 196, 64 };
		}
	});
	pause_menu.push_back(music_vol);
	AddEntityToScene(music_vol);

	// Initialize sfx volume slider
	BarUI* sfx_vol = new BarUI({ Utils::GetWorldWidth() * 0.5f, 4.f });
	sfx_vol->text = "";
	sfx_vol->SetValue(Game::SfxVol());
	sfx_vol->scale = { VOL_SCALE_X * 0.75f, VOL_SCALE_Y * 0.35f };
	sfx_vol->text_size = 7.5f;
	sfx_vol->text_alignment = BaseUI::TextAlignment::LEFT_CORNER;
	sfx_vol->AddUpdateListener(this, [sfx_vol](const f32&) {
		// Update group volume based on slider value when interacted with
		AEAudioSetGroupVolume(Game::GetSfxGroup(), AEClamp(sfx_vol->GetValue(), 0.f, 1.f));
		Game::SfxVol() = sfx_vol->GetValue();

		// Update text on slider
		char vol[64];
		sprintf_s(vol, 64, "SFX Volume: %0.2f", sfx_vol->GetValue() * 100.f);
		sfx_vol->text = vol;

		// Change slider color based on interaction
		if (sfx_vol->IsDragging()) {
			sfx_vol->overlay_color = { 255, 64, 128, 64 };
		}
		else {
			sfx_vol->overlay_color = { 255, 64, 196, 64 };
		}
	});
	pause_menu.push_back(sfx_vol);
	AddEntityToScene(sfx_vol);

	// Listen to button click to advance to the next level
	advance->AddClickListener([this, pause_menu](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			game_state = GameState::PLAYING;
			for (BaseUI* en : pause_menu) {
				en->active = false;
			}
			AEAudioResumeGroup(Game::GetMusicGroup());
		}
	});

	for (BaseUI* en : pause_menu) {
		en->active = false;
	}

	// Add input listener when ESC to pressed to toggle pause menu
	InputEvent::Listeners += {this, [pause_menu, this](const InputEvent* ev) {
		if (ev->IsKeyTriggered(AEVK_ESCAPE)) {
			if (game_state == GameState::PLAYING) {
				game_state = GameState::PAUSE;
				for (BaseUI* en : pause_menu) {
					en->active = true;
				}
				AEAudioPauseGroup(Game::GetMusicGroup());
			}
			else if (game_state == GameState::PAUSE) {
				game_state = GameState::PLAYING;
				for (BaseUI* en : pause_menu) {
					en->active = false;
				}
				AEAudioResumeGroup(Game::GetMusicGroup());
			}
		}
	}};

	// Initialize background image
	ImageUI* bgd = new ImageUI{ ASSET_BACKGROUND_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f} };
	bgd->layer = BaseEntity::RenderLayer::BACKGROUND;
	bgd->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	AddEntityToScene(bgd);

	// Load level based on level file
	std::string filename = "Assets/level_";
	filename += std::string{ static_cast<char>('0' + LevelManager::GetLevel()) };
	filename += ".dat";
	std::vector<Serialization::SerializedEntity> ens = Serialization::LoadFromFile(filename.c_str());

	// Unserialize all entity data from level file
	PlayerEntity* player = nullptr;
	if (!ens.empty()) {
		for (Serialization::SerializedEntity const& sen : ens) {
			BaseEntity* en = Serialization::Unserialize(sen);
			if (en) { // If it's a valid entity
				AddEntityToScene(en);
				if (PlayerEntity* p = dynamic_cast<PlayerEntity*>(en)) {
					player = p; // Set the player pointer when loaded (only 1 should exist) 
				}
			}
		}
		std::printf("Loaded from file");
	}
	else { // No level file, create generic base level
		GameObjectEntity* wall = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_WALL, AEVec2{ 1.f, 1.f });
		wall->mesh = MeshRenderer::GetCenterRectMesh();
		wall->scale = { 30.f, 1.f };
		AddEntityToScene(wall);	
	}

	if (player == nullptr) {
		player = new PlayerEntity({ 1.f, 5.f });
		AddEntityToScene(player);
	}

	// Lock weapons based on level
	WeaponEntity* w = new TurboFistWeapon(AEVec2{ 0.f, 0.f }, player->pBody->mass);
	AddEntityToScene(w);
	player->AddWeapon(w);
	if (LevelManager::GetLevel() > 0) {
		WeaponEntity* w2 = new GrappleFistWeapon(AEVec2{ 0.f, 0.f });
		AddEntityToScene(w2);
		player->AddWeapon(w2);
	}
	if (LevelManager::GetLevel() > 1) {
		WeaponEntity* w3 = new FingerGunWeapon(AEVec2{ 0.f, 0.f });
		AddEntityToScene(w3);
		player->AddWeapon(w3);
	}
	player->SwitchWeapon(0);

	// Initialize in world weapon UI elements, the current charge / cooldown of the weapon
	BarUI* power = new BarUI{ AEVec2{ 0.f, 0.f } };
	power->scale = { 2.f, .25f };
	power->text = "";
	power->SetInteractive(false);
	power->SetValue(0.5f);
	power->layer = BaseUI::RenderLayer::PLAYER;
	power->text_size = 7.f;
	AddEntityToScene(power);

	// Listen to current weapon charge to update slider value
	power->AddUpdateListener(this, [power, player](const f32&) {
		// Update UI based on current weapon position and value
		WeaponEntity* current = player->CurrentWeapon();
		if (current == nullptr) 
			return;

		// Use cooldown timer as priority
		if (current->GetCooldownTimer() > 0) {
			power->SetValue(current->GetCooldownTimer() / current->GetCooldownDuration());
			power->overlay_color = { 255, 128, 128, 128 };
		}
		else {
			power->SetValue(current->GetChannelTimer() / current->GetMaxChannelTime());
			power->overlay_color = { 255, 255, 0, 0 };
		}

		// Always render below the weapon
		power->position = current->position;
		power->position.y -= std::abs(current->scale.y) * 0.65f;
	});

	// Initialize player movement interaction
	// Uses a listener so that when the level is finished, we can remove the player interaction
	player->AddUpdateListener(this, [player](const f32& dt) {
		if (dt == 0)
			return;
		if (player->time_elapsed_since_last_damage > PLAYER_CONTROL_LOCK_AFTER_HIT) {
			AEVec2 dir{};
			if (AEInputCheckCurr(AEVK_A)) {
				dir += { -1.f, 0.f };
			}
			if (AEInputCheckCurr(AEVK_D)) {
				dir += { 1.f, 0.f };
			}
			if (dir.x || dir.y) {
				AEVec2Normalize(&dir, &dir);
			}

			if (dir.x /*&& player->pBody->is_standing_above*/) {
				player->Move(dir);
			}
		}

		// Swap weapon
		if (AEInputCheckCurr(AEVK_1)) {
			player->SwitchWeapon(0);
		}
		else if (AEInputCheckCurr(AEVK_2)) {
			player->SwitchWeapon(1);
		}
		else if (AEInputCheckCurr(AEVK_3)) {
			player->SwitchWeapon(2);
		}

		if (AEInputCheckCurr(AEVK_SPACE) && player->pBody->is_standing_above) {
			player->Jump();
		}
	});

	// PlayerEntity listener for camera tracking
	// Uses the AssetManager instance as owner as it's perpetual lifetime
	player->AddUpdateListener(AssetManager::GetInstance(), [this, player](const f32&) {
		if (BossEntity * e = dynamic_cast<BossEntity*>(GetFirstEntityOfType<BossEntity>())) {
			if (e->GetBossActivated()) {
				camManager->SetTarget(Utils::WorldToScreen(e->GetBossRoomCenter().x, e->GetBossRoomCenter().y).x, 0);
				return;
			}
		}
		if (game_state == GameState::PLAYING)
			camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
	});

	// Initialize HUD background
	ImageUI* hud = new ImageUI{ ASSET_HUD_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() - 1.7f} };
	hud->scale = { Utils::GetWorldWidth() - 5.f, 3.f };
	hud->color.a = 210;
	hud->SetInteractive(false);
	AddEntityToScene(hud);

	// Initialize Space, A, D & Mouse hotkey displays
	AddEntityToScene(CreateHotKeyDisplay({ Utils::GetWorldWidth() - 4.5f, Utils::GetWorldHeight() - 1.f }, "LB", AEVK_LBUTTON));
	AddEntityToScene(CreateHotKeyDisplay({ Utils::GetWorldWidth() - 5.5f, Utils::GetWorldHeight() - 2.f }, 'A', AEVK_A));
	AddEntityToScene(CreateHotKeyDisplay({ Utils::GetWorldWidth() - 3.5f, Utils::GetWorldHeight() - 2.f }, 'D', AEVK_D));
	AddEntityToScene(CreateHotKeyDisplay({ Utils::GetWorldWidth() - 4.5f, Utils::GetWorldHeight() - 2.f }, "Sp", AEVK_SPACE));

	// Initialize player health HUD element
	BarUI* player_health = new BarUI( {Utils::GetWorldWidth() * 0.2f, Utils::GetWorldHeight() - 1.5f} );
	player_health->text_alignment = BaseUI::TextAlignment::LEFT_CORNER;
	player_health->text_size = 5.f;
	player_health->scale = { 12.f, 1.5f };
	player_health->overlay_color = { 255, 64, 255, 64 };
	player_health->color = { 255, 255, 64, 64 };
	player_health->layer = BaseUI::RenderLayer::UI;
	player_health->SetInteractive(false);

	// Listen to player health to update slider value
	player_health->AddUpdateListener(this, [player, player_health](const f32&) {
		if (player == nullptr) {
			return;
		}
		player_health->SetValue(player->health / player->max_health);
		char health[64];
		sprintf_s(health, 64, "Health: %0.2f / %0.2f", player->health, player->max_health);
		player_health->text = health;
	});
	AddEntityToScene(player_health);

	// Initialize weapon charge HUD element
	BarUI* weaponhud = new BarUI{ {Utils::GetWorldWidth() * 0.49f, Utils::GetWorldHeight() - 1.5f} };
	weaponhud->text_alignment = BaseUI::TextAlignment::LEFT_CORNER;
	weaponhud->text_size = 5.f;
	weaponhud->scale = { 12.f, 1.5f };
	weaponhud->text = "";
	weaponhud->SetInteractive(false);
	weaponhud->layer = BaseUI::RenderLayer::UI;
	AddEntityToScene(weaponhud);

	BaseEntity* en = weaponhud;
	en->Update(2.f);

	// Listen to current weapon charge to update slider value
	weaponhud->AddUpdateListener(this, [weaponhud, player](const f32&) {
		WeaponEntity* current = player->CurrentWeapon();
		if (current == nullptr) {
			weaponhud->text = "";
			return;
		}
		weaponhud->overlay_color = { 255, 255, 0, 0 };
		if (current->GetCooldownTimer() > 0) {
			weaponhud->SetValue(current->GetCooldownTimer() / current->GetCooldownDuration());
			weaponhud->overlay_color = { 255, 128, 128, 128 };
			char cooldown[64];
			sprintf_s(cooldown, 64, "Cooldown: %0.2f%%", (current->GetCooldownTimer() / current->GetCooldownDuration()) * 100.f);
			weaponhud->text = cooldown;
		}
		else {
			weaponhud->SetValue(current->GetChannelTimer() / current->GetMaxChannelTime());
			char charge[64];
			sprintf_s(charge, 64, "Charge: %0.2f%%", (current->GetChannelTimer() / current->GetMaxChannelTime()) * 100.f);
			weaponhud->text = charge;
		}
	});

	// Initialize coins HUD element
	ImageUI* coins = new ImageUI{ ASSET_SMALLBUTTON_IMAGE, {Utils::GetWorldWidth() * 0.69f, Utils::GetWorldHeight() - 1.5f} };
	coins->text_size = 7.5f;
	coins->scale = {5.f, 2.f};

	// Listen to how many coins were collected to update text display
	coins->AddUpdateListener(this, [coins, player](const f32&) {
		char collected[64];
		sprintf_s(collected, 64, "Coins: %d", player->Coins());
		coins->text = collected;
	});
	AddEntityToScene(coins);

	// Initialize game time HUD element
	ImageUI* time = new ImageUI{ ASSET_SMALLBUTTON_IMAGE, {Utils::GetWorldWidth() * 0.81f, Utils::GetWorldHeight() - 1.5f} };
	time->text_size = 7.5f;
	time->scale = { 5.f, 2.f };

	// Listen to current game time to update text display
	time->AddUpdateListener(this, [time, this](const f32&) {
		char timer[64];
		sprintf_s(timer, 64, "Timer: %0.2f", game_timer);
		time->text = timer;
	});
	AddEntityToScene(time);

	// Load any tutorial UI elements based on the current level
	LevelManager::LoadTutorial(this);

	// Everything has finished initializing, we can start
	Game::SetBackgroundColor({ 1.f, 0.3f, 0.3f, 0.3f });
	game_state = GameState::PLAYING;

	// Play looped game music
	AEAudio audio = AssetManager::GetAudio(ASSET_GAMEBGM_AUDIO);
	AEAudioPlay(audio, Game::GetMusicGroup(), 1.f, 1.f, -1);
}

/*!
* @brief Inherited: PreUpdate all entities in the scene
* @param dt - The delta time for this frame
*/
void GameScene::PreUpdate(const f32& dt) {
	BaseScene::PreUpdate(game_state != GameState::PLAYING ? 0.f : dt); // Base update all entities
}

/*!
* @brief Inherited: Update all entities in the scene
* @param dt - The delta time for this frame
*/
void GameScene::Update(const f32& dt) {
	// Update current static entities
	staticEntities = SceneManager::GetInstance()->GetCurrentScene()->GetBaseEntitiesOfType<StaticEntity>();
	BaseScene::Update(game_state != GameState::PLAYING ? 0.f : dt); // Base update all entities
	staticEntities.clear();

	// Update game time if not lost or won
	if (game_state == GameState::PLAYING)
		game_timer += dt;
}

/*!
* @brief Inherited: PostUpdate all entities in the scene
* @param dt - The delta time for this frame
*/
void GameScene::PostUpdate(const f32& dt) {
	BaseScene::PostUpdate(game_state != GameState::PLAYING ? 0.f : dt); // Base post update all entities

	// Previous game state was lost, we reset the level
	if (game_state == GameState::LOST) {
		End();
		Init();
		return;
	}

	// Lose when player dies
	PlayerEntity* player = GetFirstEntityOfType<PlayerEntity>();
	if (player == nullptr || player->health <= 0.f)
		Lose();
}

/*!
* @brief Inherited: Clear the entities from the scene.
*/
void GameScene::End() {
	BaseScene::End();
	InputEvent::Listeners -= this;
}

/*!
* @brief Called when the current level has been won
*/
void GameScene::Win() {
	game_state = GameState::WON;
	LevelManager::SetLevelTime(LevelManager::GetLevel(), game_timer); // set this level time
	LevelManager::UnlockLevel(LevelManager::GetLevel() + 1); // unlock next level
	LevelManager::SavePlayerData(); // save data

	// If this game scene has a player, show the win screen
	PlayerEntity* p = GetFirstEntityOfType<PlayerEntity>();
	if (!p)
		return;

	// Initialize win screen background
	ImageUI* bgd = new ImageUI{ ASSET_HUD_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f} };
	bgd->scale = { Utils::GetWorldWidth() * 0.3f, Utils::GetWorldHeight() * 0.9f };
	AddEntityToScene(bgd);

	// Initialize win screen text
	ButtonUI* toptext = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.7f } };
	toptext->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	toptext->text_size = 10.f;
	toptext->color.a = 0;
	int score = static_cast<int>((p->Coins() / game_timer) * 100.f);
	std::ostringstream oss;
	oss << "You beat this level!     |Score: " << score << "     ";
	toptext->text = oss.str();
	toptext->SetInteractive(false);
	AddEntityToScene(toptext);

	// Initialize next level button
	ButtonUI* advance = new ButtonUI{{ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }};
	advance->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	advance->text_size = 10.f;
	advance->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	advance->text = "Next Level  ";

	// Listen to button click to advance to the next level
	advance->AddClickListener([this](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			LevelManager::SetLevel(LevelManager::GetLevel() + 1);
			Lose(); // Basically reset the current state with the new level
		}
	});
	AddEntityToScene(advance);

	// Initialize the return to main menu button
	ButtonUI* back = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.35f } };
	back->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	back->text_size = 10.f;
	back->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	back->text = "Back to Menu   ";

	// Listen to button click to go back to main menu scene
	back->AddClickListener([this](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
		}
	});
	AddEntityToScene(back);

	// Remove player input listener
	p->RemoveUpdateListener(this);
}

/*!
* @brief Called when the current level is ready to reset
*/
void GameScene::Lose() {
	game_state = GameState::LOST;
	AEAudioStopGroup(Game::GetMusicGroup());
}

/*!
* @brief Get the current game state
* @return The current enumerator of game state actively
*/
GameState GameScene::GetGameState() const {
	return game_state;
}

/*!
* @brief Get the vector of all static entities in the scene
* @return A vector of static entity pointers
*/
std::vector<BaseEntity*>& GameScene::GetStaticEntities() {
	return staticEntities;
}
