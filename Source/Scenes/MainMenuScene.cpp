#include "MainMenuScene.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../UI/ButtonUI.hpp"

MainMenuScene::MainMenuScene() {
}

MainMenuScene::~MainMenuScene() {
}

void MainMenuScene::Init() {
	std::shared_ptr<BaseEntity> p = std::make_shared<Player>();
	scene_entities.push_back(p);

	std::shared_ptr<BaseEntity> s = std::make_shared<ButtonUI>(AEVec2{8.f, 4.5f});
	s.get()->scale.x = 5.f;
	scene_entities.push_back(s);
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