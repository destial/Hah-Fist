#include "MainMenuScene.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"

MainMenuScene::MainMenuScene() {
}

MainMenuScene::~MainMenuScene() {
}

void MainMenuScene::Init() {
	Player* p = new Player({ 0.f, 0.f });
	p->scale = { 50.f, 50.f };
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