#include "StartMenuScene.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/ImageUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/LevelManager.hpp"
#include <cstdio>
#include <string>

static ButtonUI* CreateButtonDisplay(AEVec2 pos, const char* ch) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture("Assets/Icons/small_button_grey.png");
	b->scale = { 12.5f, 4.f };
	b->color = { 255, 255, 255, 255 };
	b->overlay_color = { 255, 128, 128, 128 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->text = ch;
	b->text_size = 7.5f;
	b->text_alignment = BaseUI::TEXT_ALIGNMENT::CENTER;
	return b;
}

StartMenuScene::StartMenuScene() : BaseScene() {}

StartMenuScene::~StartMenuScene() {}

void StartMenuScene::Init() {
	static f32 level_panel = 0.f;
	static bool panel_active = false;

	ButtonUI* start = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, "Play  ");
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
		level_panel = min_max(level_panel, 0.f, 1.f);
	});
	start->AddUpdateListener(this, [start](const f32& dt){
		start->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	ButtonUI* credits = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f - 5.5f }, "Credits   ");
	credits->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			SceneManager::GetInstance()->SetNextScene(Scenes::CREDITS);
		}
	});
	credits->AddUpdateListener(this, [credits](const f32& dt) {
		credits->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel);
	});

	ButtonUI* quit = CreateButtonDisplay({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f - 11.f }, "Quit  ");
	quit->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			Game::SetGameRunning(false);
		}
	});
	quit->AddUpdateListener(this, [quit](const f32& dt) {
		quit->position.x = Utils::LerpCircle(Utils::GetWorldWidth() * 0.5f, -Utils::GetWorldWidth() * 0.5f, level_panel); 
	});

	ImageUI* title = new ImageUI{ "Assets/title.png", AEVec2{Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f + 6.5f}, 6, 5 };
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

	for (int i = 0; i < 3; ++i) {
		float w = Utils::GetWorldWidth() + 2.f + (i * 3.f);
		ButtonUI* level_button = CreateButtonDisplay({ w, Utils::GetWorldHeight() * 0.5f }, std::string{ static_cast<char>('1' + i) }.c_str());
		level_button->scale = { 2.f, 2.f };
		scene_entities.push_back(level_button);
		level_button->AddClickListener([i](BaseUI::MouseButton b) {
			const int l = i;
			if (b & BaseUI::MouseButton::LEFT) {
				LevelManager::SetLevel(l);
				SceneManager::GetInstance()->SetNextScene(Scenes::GAME);
			}
		});
		level_button->AddUpdateListener(this, [level_button, w](const f32& dt) {
			level_button->position.x = Utils::LerpCircle(w, w - Utils::GetWorldWidth(), level_panel);
		});
	}

	scene_entities.push_back(start);
	scene_entities.push_back(quit);
	scene_entities.push_back(back);
	scene_entities.push_back(credits);
	scene_entities.push_back(title);
	Game::SetBackgroundColor(Color{ 1.f, 0.3f, 0.3f, 0.3f });

	/*AEAudio audio = AssetManager::GetAudio("Assets/bouken.mp3");
	AEAudioPlay(audio, Game::GetMusicGroup(), 1.f, 1.f, 1);*/
}