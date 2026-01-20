#include "MainMenuScene.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/GameObjectEntity.hpp"
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
	for (int y = 0; y < Utils::GetWorldHeight(); y += 5) {
		for (int x = 0; x < Utils::GetWorldWidth(); x += 5) {
			std::shared_ptr<ButtonUI> s = std::make_shared<ButtonUI>(AEVec2{ static_cast<f32>(x), static_cast<f32>(y) });
			int red = 255;
			int green = 255;
			int blue = 12;
			s->base_color = Utils::ConvertToColor(red, green, blue);

			char b[10];
			sprintf_s(b, "%d,%d", x, y);
			s->text_size = 10.f;

			s->AddClickListener([this, x, y]() {
				std::printf("button pressed! %d, %d\n", x, y);
			});

			scene_entities.push_back(s);
		}
	}

	std::shared_ptr<GameObjectEntity> p = std::make_shared<Player>(AEVec2{ 0.f, 0.f });
	scene_entities.push_back(p);
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