#include "StartMenuScene.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/Constant.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/ImageUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/LevelManager.hpp"
#include <cstdio>
#include <string>

static ButtonUI* CreateButtonDisplay(AEVec2 pos, const char* ch) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	b->scale = { 12.5f, 4.f };
	b->color = { 255, 255, 255, 255 };
	b->overlay_color = { 255, 128, 128, 128 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->text = ch;
	b->text_size = 15.f;
	b->text_alignment = BaseUI::TEXT_ALIGNMENT::CENTER;
	return b;
}

StartMenuScene::StartMenuScene() : BaseScene() {}

StartMenuScene::~StartMenuScene() {}

void StartMenuScene::Init() {
	static f32 level_panel = 0.f;
	static bool panel_active = false;

	ImageUI* fist = new ImageUI{ ASSET_TURBOFIST_IMAGE, { 0, Utils::GetWorldHeight() * 0.5f } };
	fist->AddUpdateListener(this, [fist](const f32& dt) {
		if (fist->scale.x > 0) {
			fist->position.x += dt * 5.f;
			if (fist->position.x + fist->scale.x * 0.5f > Utils::GetWorldWidth()) {
				fist->scale.x *= -1;
			}
		}
		else {
			fist->position.x -= dt * 5.f;
			if (fist->position.x + fist->scale.x * 0.5f < 0) {
				fist->scale.x *= -1;
			}
		}
	});
	fist->scale = { 5.f, 5.f };

	ImageUI* fist2 = new ImageUI{ ASSET_GRAPPLEFIST_IMAGE, { 10.f, Utils::GetWorldHeight() * 0.25f } };
	fist2->AddUpdateListener(this, [fist2](const f32& dt) {
		if (fist2->scale.x > 0) {
			fist2->position.x += dt * 5.f;
			if (fist2->position.x + fist2->scale.x * 0.5f > Utils::GetWorldWidth()) {
				fist2->scale.x *= -1;
			}
		}
		else {
			fist2->position.x -= dt * 5.f;
			if (fist2->position.x + fist2->scale.x * 0.5f < 0) {
				fist2->scale.x *= -1;
			}
		}
	});
	fist2->scale = { 5.f, 5.f };

	ImageUI* fist3 = new ImageUI{ ASSET_FINGERGUN_IMAGE, { -10.f, Utils::GetWorldHeight() * 0.75f } };
	fist3->AddUpdateListener(this, [fist3](const f32& dt) {
		if (fist3->scale.x > 0) {
			fist3->position.x += dt * 5.f;
			if (fist3->position.x + fist3->scale.x * 0.5f > Utils::GetWorldWidth()) {
				fist3->scale.x *= -1;
			}
		}
		else {
			fist3->position.x -= dt * 5.f;
			if (fist3->position.x + fist3->scale.x * 0.5f < 0) {
				fist3->scale.x *= -1;
			}
		}
	});
	fist3->scale = { 5.f, 5.f };

	scene_entities.push_back(fist);
	scene_entities.push_back(fist2);
	scene_entities.push_back(fist3);

	ButtonUI* start = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, "Play     ");
	start->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			panel_active = true;
		}
	});
	start->AddPreUpdateListener(this, [start](const f32& dt) {
		if (panel_active) {
			level_panel += dt;
		}
		else {
			level_panel -= dt;
		}
		level_panel = AEClamp(level_panel, 0.f, 1.f);
	});
	start->AddUpdateListener(this, [start](const f32& dt){
		start->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	ButtonUI* credits = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f - 5.5f }, "Credits         ");
	credits->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			SceneManager::GetInstance()->SetNextScene(Scenes::CREDITS);
		}
	});
	credits->AddUpdateListener(this, [credits](const f32& dt) {
		credits->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	ButtonUI* quit = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f - 11.f }, "Quit      ");
	quit->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			Game::SetGameRunning(false);
		}
	});
	quit->AddUpdateListener(this, [quit](const f32& dt) {
		quit->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel); 
	});

	ImageUI* title = new ImageUI{ ASSET_TITLE_SPRITE, AEVec2{Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f + 6.5f}, 6, 5 };
	title->SetSpriteDuration(2.f);
	title->scale = { 25.f, 15.f };
	title->SetInteractive(false);
	title->AddUpdateListener(this, [title](const f32& dt) {
		title->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	ButtonUI* back = CreateButtonDisplay({ Utils::GetWorldWidth() + 2.5f, Utils::GetWorldHeight() - 5.f }, "<");
	back->scale = { 2.5f, 2.5f };
	back->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			panel_active = false;
		}
	});
	back->AddUpdateListener(this, [back](const f32& dt) {
		back->position.x = Utils::LerpCircle(Utils::GetWorldWidth() + 2.5f, Utils::GetWorldWidth() + 2.5f - Utils::GetWorldWidth(), level_panel);
	});

	LevelManager::SetLevelTime(0, 0);
	for (int i = 1; i < TOTAL_LEVELS; ++i) {
		LevelManager::SetLevelTime(i, -1);
	}

	LevelManager::LoadPlayerData();
	std::map<int, float> lvls = LevelManager::GetUnlockedLvls();
#if _DEBUG
	std::vector<ButtonUI*> buttons;
#endif
	for (int i = 0; i < TOTAL_LEVELS; ++i) {
		float w = Utils::GetWorldWidth() + 5.f + (i * 7.f);
		std::string s{ static_cast<char>('1' + i) };
		s += "  ";
		ButtonUI* level_button = CreateButtonDisplay({ w, Utils::GetWorldHeight() * 0.5f }, s.c_str());
		level_button->scale = { 5.f, 5.f };
		
		if (lvls[i] != -1) {
			level_button->AddClickListener([i](BaseUI::MouseButton b) {
				int l = i;
				if (b & BaseUI::MouseButton::LEFT) {
					LevelManager::SetLevel(l);
					SceneManager::GetInstance()->SetNextScene(Scenes::GAME);
				}
			});

			ButtonUI* time = new ButtonUI{ {w, level_button->position.y - level_button->scale.y * 0.65f} };
			char buffer[256];
			if (lvls[i]) {
				sprintf_s(buffer, 256, "Fastest: %0.2f", lvls[i]);
			}
			else {
				sprintf_s(buffer, 256, "Unlocked");
			}
			time->text = std::string{ buffer };
			time->color = { 0, 0, 0, 0 };
			time->text_size = 8.f;
			time->AddUpdateListener(this, [time, w](const f32& dt) {
				time->position.x = Utils::LerpCircle(w, w - Utils::GetWorldWidth(), level_panel);
			});

			scene_entities.push_back(time);
		}
		else {
#if _DEBUG
			buttons.push_back(level_button);
#endif
			level_button->SetInteractive(false);
			level_button->color = { 255, 255, 0, 0 };
		}
		level_button->AddUpdateListener(this, [level_button, w](const f32& dt) {
			level_button->position.x = Utils::LerpCircle(w, w - Utils::GetWorldWidth(), level_panel);
		});

		scene_entities.push_back(level_button);
	}

#if _DEBUG
	ButtonUI* unlock_all = CreateButtonDisplay({ 5.f, 5.f }, "Debug: Unlock All");
	unlock_all->scale *= 0.5f;
	unlock_all->text_size *= 0.5f;
	unlock_all->AddClickListener([buttons, unlock_all](const BaseUI::MouseButton b) {
		for (ButtonUI* level_button : buttons) {
			int l = level_button->text[0] - '1';
			LevelManager::SetLevelTime(l, 1);
			level_button->SetInteractive(true);
			level_button->AddClickListener([l, level_button](BaseUI::MouseButton b) {
				if (b & BaseUI::MouseButton::LEFT) {
					LevelManager::SetLevel(l);
					SceneManager::GetInstance()->SetNextScene(Scenes::GAME);
				}
			});
			level_button->color = { 255, 255, 255, 255 };
		}
		LevelManager::SavePlayerData();
	});
	scene_entities.push_back(unlock_all);
#endif

	scene_entities.push_back(start);
	scene_entities.push_back(quit);
	scene_entities.push_back(back);
	scene_entities.push_back(credits);
	scene_entities.push_back(title);
	Game::SetBackgroundColor(Color{ 1.f, 0.3f, 0.3f, 0.3f });

	AEAudio audio = AssetManager::GetAudio(ASSET_MAINMENUBGM_AUDIO);
	AEAudioPlay(audio, Game::GetMusicGroup(), 1.f, 1.f, -1);
}