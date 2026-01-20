#include "MainMenuScene.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../UI/ButtonUI.hpp"
#include <cstdio>

MainMenuScene::MainMenuScene() : BaseScene() {
}

MainMenuScene::~MainMenuScene() {
}

void MainMenuScene::Init() {
	ButtonUI* s = new ButtonUI(AEVec2{ 0.1f, Utils::GetWorldHeight() - 0.1f });
	s->base_color = Utils::ConvertToColor(255, 255, 255, 0);
	s->overlay_color = s->base_color;
	s->scale.x = 10.f;
	s->text_size = 5.f;
	static int fps_counter = 0;
	s->AddUpdateListener([s]() {
		if (++fps_counter > 10) {
			char b[50];
			sprintf_s(b, "FPS:%.0f", 1.f / Utils::GetDeltaTime());
			s->text = std::string(b);
			fps_counter = 0;
		}
	});
	scene_entities.push_back(s);

	BaseEntity* p = new Player(AEVec2{ 0.f, 0.f });
	scene_entities.push_back(p);

	BaseEntity* w = new Weapon(AEVec2{ 0.f, 0.f }, p);
	scene_entities.push_back(w);
}

void MainMenuScene::PreUpdate(const f32& dt) {
	BaseScene::PreUpdate(dt);
}

void MainMenuScene::Update(const f32& dt) {
	BaseScene::Update(dt);
}

void MainMenuScene::PostUpdate(const f32& dt) {
	BaseScene::PostUpdate(dt);
}

void MainMenuScene::Render() {
	BaseScene::Render();
}

void MainMenuScene::End() {
	BaseScene::End();
}