/*!
* @file StartMenuScene.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 26 January 2026
* @course CSD1451
* @brief Definition file for a start menu scene
*/

#include "StartMenuScene.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/Constant.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/BarUI.hpp"
#include "../UI/ImageUI.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/LevelManager.hpp"
#include <cstdio>
#include <string>

// Constant values for button sizing
constexpr float BUTTON_SCALE_X = 12.5f;
constexpr float BUTTON_SCALE_Y = 4.f;
constexpr float LEVEL_BUTTON_SCALE = 5.f;
constexpr float LEVEL_BUTTON_GAP = 2.f;
constexpr float BACKGROUND_FIST_SCALE = 5.f;
constexpr float BACKGROUND_FIST_SPEED = 5.f;

/*!
* @brief Create a generic button display with initialized values
* @param pos - The world position to place the button in
* @param ch - The content display on the button
*/
static ButtonUI* CreateButtonDisplay(AEVec2 pos, const char* ch) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	b->scale = { BUTTON_SCALE_X, BUTTON_SCALE_Y };
	b->color = { 255, 255, 255, 255 };
	b->overlay_color = { 255, 128, 128, 128 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->text = ch;
	b->text_size = 15.f;
	b->text_alignment = BaseUI::TextAlignment::CENTER;
	return b;
}

StartMenuScene::StartMenuScene() : BaseScene() {} // Empty ctor

StartMenuScene::~StartMenuScene() {} // Empty dtor

/*!
* @brief Inherited: Initialize the scene with UI starting points
*/
void StartMenuScene::Init() {
	// Static toggleable variables to change state between main menu and level menu
	static f32 level_panel = 0.f; // 0-1 float for smooth transition
	static bool panel_active = false; // Toggle state

	// Initialize 3 background fists
	const char* images[] = { ASSET_TURBOFIST_IMAGE, ASSET_GRAPPLEFIST_IMAGE, ASSET_FINGERGUN_IMAGE };
	f32 start_x = -BACKGROUND_FIST_SCALE * 2.f;
	f32 start_y = Utils::GetWorldHeight() * 0.75f;
	for (const char* image : images) {
		// Initialize background fist
		ImageUI* fist = new ImageUI{ image, { start_x, start_y } };
		fist->scale = { BACKGROUND_FIST_SCALE, BACKGROUND_FIST_SCALE };
		fist->AddUpdateListener(this, [fist](const f32& dt) {
			// Rebound fist if hit right border
			if (fist->scale.x > 0) {
				fist->position.x += dt * BACKGROUND_FIST_SPEED;
				if (fist->position.x + fist->scale.x * 0.5f > Utils::GetWorldWidth()) {
					fist->scale.x *= -1; // Flip x-scale
				}
			}
			// Rebound fist if hit left border
			else {
				fist->position.x -= dt * BACKGROUND_FIST_SPEED;
				if (fist->position.x + fist->scale.x * 0.5f < 0) {
					fist->scale.x *= -1; // Flip x-scale
				}
			}
		});
		AddEntityToScene(fist);

		start_x += BACKGROUND_FIST_SCALE;
		start_y -= Utils::GetWorldHeight() * 0.25f;
	}
	
	// Initalize start button
	ButtonUI* start = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, "Play     ");
	start->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) { // When clicked, toggle level menu
			panel_active = true;
		}
	});
	
	// Use this start button as the main updater for the level menu
	start->AddPreUpdateListener(this, [start](const f32& dt) {
		level_panel += panel_active ? dt : -dt;

		// Ensure clamped between 0 and 1
		level_panel = AEClamp(level_panel, 0.f, 1.f);
	});

	// Transition button based on toggled level menu status
	start->AddUpdateListener(this, [start](const f32&){
		start->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	// Initalize credits button
	ButtonUI* credits = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f - 5.5f }, "Credits         ");
	credits->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) { // When click, transition to credits scene
			SceneManager::GetInstance()->SetNextScene(Scenes::CREDITS);
		}
	});

	// Transition button based on toggled level menu status
	credits->AddUpdateListener(this, [credits](const f32&) {
		credits->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	// Initialize quit button
	ButtonUI* quit = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f - 11.f }, "Quit      ");
	quit->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) { // When clicked, quit game
			Game::SetGameRunning(false);
		}
	});

	// Transition button based on toggled level menu status
	quit->AddUpdateListener(this, [quit](const f32&) {
		quit->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel); 
	});

	// Initialize title image
	ImageUI* title = new ImageUI{ ASSET_TITLE_SPRITE, AEVec2{Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f + 6.5f}, 6, 5 };
	title->SetSpriteDuration(2.f);
	title->scale = { 25.f, 15.f };
	title->SetInteractive(false);

	// Transition title based on toggled level menu status
	title->AddUpdateListener(this, [title](const f32&) {
		title->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	// Initalize level menu back button
	ButtonUI* back = CreateButtonDisplay({ Utils::GetWorldWidth() + 2.5f, Utils::GetWorldHeight() - 5.f }, "<");
	back->scale = { 2.5f, 2.5f };
	back->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) { // When click, toggle level menu status back to main menu
			panel_active = false;
		}
	});

	// Transition button based on toggled level menu status
	back->AddUpdateListener(this, [back](const f32&) {
		back->position.x = Utils::LerpCircle(Utils::GetWorldWidth() + 2.5f, Utils::GetWorldWidth() + 2.5f - Utils::GetWorldWidth(), level_panel);
	});

	// Initialize all level data
	LevelManager::SetLevelTime(0, 0);
	for (int i = 1; i < TOTAL_LEVELS; ++i) {
		LevelManager::SetLevelTime(i, -1);
	}

	// Load level data from file "user.data"
	LevelManager::LoadPlayerData();
	std::map<int, float> lvls = LevelManager::GetUnlockedLvls();
#if _DEBUG
	std::vector<ButtonUI*> buttons;
#endif

	// Initalize level buttons based on loaded level data
	for (int i = 0; i < TOTAL_LEVELS; ++i) {
		float w = Utils::GetWorldWidth() + LEVEL_BUTTON_SCALE + (i * (LEVEL_BUTTON_SCALE + LEVEL_BUTTON_GAP));
		std::string s{ static_cast<char>('1' + i) };
		s += "  ";

		// Initalize level button
		ButtonUI* level_button = CreateButtonDisplay({ w, Utils::GetWorldHeight() * 0.5f }, s.c_str());
		level_button->scale = { LEVEL_BUTTON_SCALE, LEVEL_BUTTON_SCALE };
		if (lvls[i] != -1) {
			level_button->AddClickListener([i](BaseUI::MouseButton b) {
				int l = i;
				if (b & BaseUI::MouseButton::LEFT) { // When click, use this level and transition to game scene
					LevelManager::SetLevel(l);
					SceneManager::GetInstance()->SetNextScene(Scenes::GAME);
				}
			});

			// Initalize leaderboard text below level button
			ButtonUI* time = new ButtonUI{ {w, level_button->position.y - level_button->scale.y * 0.65f} };
			char buffer[256];
			if (lvls[i]) { // If level has been played before
				sprintf_s(buffer, 256, "Fastest: %0.2fs", lvls[i]);
			}
			else { // If level has no played data
				sprintf_s(buffer, 256, "Unlocked");
			}
			time->text = std::string{ buffer };
			time->color = { 0, 0, 0, 0 };
			time->text_size = 8.f;
			time->SetInteractive(false); // Not an actual button, just text

			// Transition text based on toggled level menu status
			time->AddUpdateListener(this, [time, w](const f32&) {
				time->position.x = Utils::LerpCircle(w, w - Utils::GetWorldWidth(), level_panel);
			});

			scene_entities.push_back(time);
		}
		else {
#if _DEBUG
			buttons.push_back(level_button);
#endif
			// This level has not been unlocked yet, so set non-interactive
			level_button->SetInteractive(false);
			level_button->color = { 255, 255, 0, 0 };
		}

		// Transition button based on toggled level menu status
		level_button->AddUpdateListener(this, [level_button, w](const f32&) {
			level_button->position.x = Utils::LerpCircle(w, w - Utils::GetWorldWidth(), level_panel);
		});

		scene_entities.push_back(level_button);
	}

#if _DEBUG
	// Initalize unlock all level button if in debug build
	ButtonUI* unlock_all = CreateButtonDisplay({ 5.f, 5.f }, "Debug: Unlock All");
	unlock_all->scale *= 0.5f;
	unlock_all->text_size *= 0.5f;
	unlock_all->AddClickListener([buttons, unlock_all](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			for (ButtonUI* level_button : buttons) { // Change behaviour of level buttons to be interactable
				int l = level_button->text[0] - '1';
				LevelManager::SetLevelTime(l, 1);
				level_button->SetInteractive(true);
				level_button->AddClickListener([l, level_button](BaseUI::MouseButton b) {
					if (b & BaseUI::MouseButton::LEFT) { // When clicked, load level and transition into game
						LevelManager::SetLevel(l);
						SceneManager::GetInstance()->SetNextScene(Scenes::GAME);
					}
					});
				level_button->color = { 255, 255, 255, 255 };
			}

			// Save player data once forced unlocked
			LevelManager::SavePlayerData();
		}
	});
	unlock_all->AddUpdateListener(this, [unlock_all](const f32&) {
		// Transition button based on toggled level menu status
		unlock_all->position.x = Utils::LerpCircle(Utils::GetWorldWidth() + 5.f, 5.f, level_panel);
	});
	scene_entities.push_back(unlock_all);
#endif

	// Initalize master volume slider
	BarUI* music_vol = new BarUI({ Utils::GetWorldWidth() - 7.5f, 5.f });
	music_vol->text = "";
	music_vol->SetValue(Game::MusicVol());
	music_vol->scale = { BUTTON_SCALE_X * 0.75f, BUTTON_SCALE_Y * 0.35f };
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

		// Transition slider based on toggled level menu status
		music_vol->position.x = Utils::LerpCircle(Utils::GetWorldWidth() - 7.5f, -7.5f, level_panel);

		// Change slider color based on interaction
		if (music_vol->IsDragging()) {
			music_vol->overlay_color = { 255, 64, 128, 64 };
		}
		else {
			music_vol->overlay_color = { 255, 64, 196, 64 };
		}
	});
	scene_entities.push_back(music_vol);

	// Initialize sfx volume slider
	BarUI* sfx_vol = new BarUI({ Utils::GetWorldWidth() - 7.5f, 3.f });
	sfx_vol->text = "";
	sfx_vol->SetValue(Game::SfxVol());
	sfx_vol->scale = { BUTTON_SCALE_X * 0.75f, BUTTON_SCALE_Y * 0.35f };
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

		// Transition slider based on toggled level menu status
		sfx_vol->position.x = Utils::LerpCircle(Utils::GetWorldWidth() - 7.5f, -7.5f, level_panel);

		// Change slider color based on interaction
		if (sfx_vol->IsDragging()) {
			sfx_vol->overlay_color = { 255, 64, 128, 64 };
		}
		else {
			sfx_vol->overlay_color = { 255, 64, 196, 64 };
		}
	});
	scene_entities.push_back(sfx_vol);

	// Add all the UI elements into the scene
	scene_entities.push_back(start);
	scene_entities.push_back(quit);
	scene_entities.push_back(back);
	scene_entities.push_back(credits);
	scene_entities.push_back(title);

	// Initialize background image
	ImageUI* bgd = new ImageUI{ ASSET_BACKGROUND_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f} };
	bgd->layer = BaseEntity::RenderLayer::BACKGROUND;
	bgd->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	bgd->color = {255, 220, 220, 220};
	AddEntityToScene(bgd);

	Game::SetBackgroundColor({ 1.f, 0.3f, 0.3f, 0.3f });

	// Play looped main menu music
	AEAudio audio = AssetManager::GetAudio(ASSET_MAINMENUBGM_AUDIO);
	AEAudioPlay(audio, Game::GetMusicGroup(), 1.f, 1.f, -1);
}