#include "MainMenuScene.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"

MainMenuScene::MainMenuScene() {
}

MainMenuScene::~MainMenuScene() {

}

void MainMenuScene::Init() {
	static Player p({ 0.f, 0.f });
	p.scale = { 50.f, 50.f };
	scene_entities.push_back(&p);
}

void MainMenuScene::PreUpdate(const f32& dt) {
	scene_entities[0]->velocity = { 0.f, 0.f };

	if (AEInputCheckCurr(AEVK_W)) {
		++scene_entities[0]->velocity.y;
	}
	if (AEInputCheckCurr(AEVK_S)) {
		--scene_entities[0]->velocity.y;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		--scene_entities[0]->velocity.x;
	}
	if (AEInputCheckCurr(AEVK_D)) {
		++scene_entities[0]->velocity.x;
	}
}

void MainMenuScene::Update(const f32& dt) {

}

void MainMenuScene::PostUpdate(const f32& dt) {
	scene_entities[0]->position += scene_entities[0]->velocity;
}

void MainMenuScene::Render() {
	BaseScene::Render();
}

void MainMenuScene::End() {

}