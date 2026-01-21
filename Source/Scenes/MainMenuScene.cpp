#include "MainMenuScene.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/EnemyEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include <cstdio>
#include <string>

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, char ch) {
	ButtonUI* b = new ButtonUI(pos);
	b->texture = AssetManager::GetTexture("Assets/Icons/small_button_grey.png");
	b->color = { 255, 255, 255, 255 };
	b->overlay_color = b->color;
	b->text = ch;
	b->text_size = 4.f;
	return b;
}

MainMenuScene::MainMenuScene() : BaseScene() {
}

MainMenuScene::~MainMenuScene() {
}

void MainMenuScene::Init() {
	ButtonUI* s = new ButtonUI(AEVec2{ 3.f, Utils::GetWorldHeight() - .6f });
	s->color = { 0, 255, 255, 255 };
	s->overlay_color = s->color;
	s->scale.x = 5.5f;
	s->text_size = 5.f;
	s->text = "FPS:";
	static float fps_counter = 0.f;
	s->AddUpdateListener(this, [s]() {
		if ((fps_counter += Utils::GetDeltaTime()) > 0.2f) {
			char b[50];
			sprintf_s(b, "FPS:%.0f", 1.f / Utils::GetDeltaTime());
			s->text = std::string(b);
			fps_counter = 0.f;
		}
	});
	scene_entities.push_back(s);

	ButtonUI* display = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 1.f, Utils::GetWorldHeight() - 1.f }, 'W');
	scene_entities.push_back(display);

	GameObjectEntity* p = new Player(AEVec2{ 0.f, 0.f });
	GameObjectEntity* e = new EnemyEntity(AEVec2{ 9.f, 4.5f });
	e->AddPreUpdateListener(this, [e]() {
		e->color = { 255, 255, 255, 255 };
	});
	scene_entities.push_back(p);
	scene_entities.push_back(e);
	gameObjects.push_back(p);
	gameObjects.push_back(e);

	BaseEntity* w = new Weapon(AEVec2{ 0.f, 0.f }, p);
	scene_entities.push_back(w);
}

void MainMenuScene::PreUpdate(const f32& dt) {
	BaseScene::PreUpdate(dt);
}

void MainMenuScene::Update(const f32& dt) {
	BaseScene::Update(dt);
	// Collision detection 
	for (std::vector<GameObjectEntity*>::iterator it = gameObjects.begin(); it != gameObjects.end(); ++it) {
		GameObjectEntity* go = static_cast<GameObjectEntity*>(*it);
		if (go->isActive) {
			if (go->type == GameObjectEntity::AABB) {
				for (std::vector<GameObjectEntity*>::iterator it2 = gameObjects.begin(); it2 != gameObjects.end(); ++it2) {
					GameObjectEntity* go2 = static_cast<GameObjectEntity*>(*it2);
					if (go2 == go) continue;
					if (Utils::AABB(go->position, go->scale.x, go->scale.y, go2->position, go2->scale.x, go2->scale.y)) {
						// Collision response WIP
						go->color = { 255, 0, 0, 0 };
					}
				}
			}
		}
	}
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