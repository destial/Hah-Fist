#include "StartMenuScene.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/EnemyEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../Managers/SceneManager.hpp"
#include <cstdio>
#include <string>

static ButtonUI* CreateButtonDisplay(AEVec2 pos, const char* ch) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture("Assets/Icons/small_button_grey.png");
	b->scale = { 10.f, 7.f };
	b->color = { 255, 255, 255, 255 };
	b->overlay_color = { 255, 128, 128, 128 };
	b->text = ch;
	b->text_size = 7.5f;
	b->text_alignment = BaseUI::TEXT_ALIGNMENT::CENTER;
	return b;
}

StartMenuScene::StartMenuScene() : BaseScene() {
}

StartMenuScene::~StartMenuScene() {
}

void StartMenuScene::Init() {
	ButtonUI* s = new ButtonUI(AEVec2{ 3.f, Utils::GetWorldHeight() - .6f });
	s->color = { 0, 0, 0, 0 };
	s->overlay_color = s->color;
	s->scale.x = 5.5f;
	s->text_size = 5.f;
	s->text = "FPS:";
	s->text_alignment = BaseUI::TEXT_ALIGNMENT::LEFT_CORNER;
	static float fps_counter = 0.f;
	s->AddUpdateListener(this, [s]() {
		if ((fps_counter += Utils::GetDeltaTime()) > 0.1f) {
			char b[50];
			sprintf_s(b, "FPS:%.0f", 1.f / Utils::GetDeltaTime());
			s->text = std::string(b);
			fps_counter = 0.f;
		}
	});

	ButtonUI* wk = CreateButtonDisplay(AEVec2{ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, "Start  ");
	wk->AddClickListener([](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			SceneManager::GetInstance()->SetNextScene(Scenes::GAME);
		}
	});

	scene_entities.push_back(wk);
	scene_entities.push_back(s);
}

void StartMenuScene::PreUpdate(const f32& dt) {
	BaseScene::PreUpdate(dt);
}

void StartMenuScene::Update(const f32& dt) {
	BaseScene::Update(dt);
}

void StartMenuScene::PostUpdate(const f32& dt) {
	BaseScene::PostUpdate(dt);
}

void StartMenuScene::Render() {
	BaseScene::Render();
}

void StartMenuScene::End() {
	BaseScene::End();
}