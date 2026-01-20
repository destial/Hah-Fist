#include "MainMenuScene.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../UI/ButtonUI.hpp"
#include <cstdio>

MainMenuScene::MainMenuScene() {
}

MainMenuScene::~MainMenuScene() {
}

void MainMenuScene::Init() {
	std::shared_ptr<BaseEntity> p = std::make_shared<Player>(AEVec2{0.f, 0.f});
	scene_entities.push_back(p);

	for (int y = 0; y < Utils::GetWorldHeight(); y += 5) {
		for (int x = 0; x < Utils::GetWorldWidth(); x += 5) {
			std::shared_ptr<ButtonUI> s = std::make_shared<ButtonUI>(AEVec2{ static_cast<f32>(x), static_cast<f32>(y) });
			int red = static_cast<int>(AERandFloat() * 255);
			int green = static_cast<int>(AERandFloat() * 255);
			int blue = static_cast<int>(AERandFloat() * 255);
			s.get()->base_color = Utils::ConvertToColor(red, green, blue);

			char b[10];
			sprintf_s(b, "%d,%d", x, y);
			s.get()->text = "a";
			s.get()->text_size = 50.f;

			s.get()->AddClickListener([this, x, y]() {
				std::printf("button pressed! %d, %d\n", x, y);
			});

			scene_entities.push_back(s);
		}
	}
	
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