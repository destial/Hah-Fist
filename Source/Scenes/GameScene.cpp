#include "GameScene.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/EnemyEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../UI/Debug.hpp"
#include "AEMath.h"
#include <cstdio>
#include <string>

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, char ch) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture("Assets/Icons/small_button_grey.png");
	b->color = { 255, 255, 255, 255 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->overlay_color = b->color;
	b->text = ch;
	b->text_size = 7.5f;
	b->font = AssetManager::GetFontId("Assets/Jersey25-Regular.ttf");
	return b;
}

GameScene::GameScene() : BaseScene() {
}

GameScene::~GameScene() {
}

static void OnGameExit(const InputEvent* ev) {
	if (ev->IsKeyTriggered(AEVK_ESCAPE)) {
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}

void GameScene::Init() {
	InputEvent::Listeners += OnGameExit;

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

	ButtonUI* cam = new ButtonUI(AEVec2{ 3.f, Utils::GetWorldHeight() - 1.5f });
	cam->color = { 0, 0, 0, 0 };
	cam->overlay_color = cam->color;
	cam->scale.x = 5.5f;
	cam->text_size = 5.f;
	cam->text = "Cam:";
	cam->text_alignment = BaseUI::TEXT_ALIGNMENT::LEFT_CORNER;
	cam->AddUpdateListener(this, [cam]() {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		char b[128];
		sprintf_s(b, "Cam:%.2f,%.2f", x, y);
		cam->text = std::string(b);
	});

	scene_entities.push_back(s);
	scene_entities.push_back(cam);

	ButtonUI* wk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 1.f }, 'W');
	ButtonUI* ak = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 3.f, Utils::GetWorldHeight() - 2.f }, 'A');
	ButtonUI* sk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 2.f }, 'S');
	ButtonUI* dk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 1.f, Utils::GetWorldHeight() - 2.f }, 'D');

	scene_entities.push_back(wk);
	scene_entities.push_back(ak);
	scene_entities.push_back(sk);
	scene_entities.push_back(dk);

	GameObjectEntity* p = new Player({ 1.f, 1.f });
	std::printf("Player mass :%f\n", p->pBody->mass);
	GameObjectEntity* e = new EnemyEntity({ 9.f, 4.5f });
	e->pBody->mass = 40.0f;
	std::printf("Enemy mass :%f\n", e->pBody->mass);
	e->AddPreUpdateListener(this, [e]() {
		e->color = { 255, 255, 255, 255 };
	});
	e->AddUpdateListener(this, [e]() {
		if (e->pBody->is_colliding) {
			e->color = { 255, 0, 0, 0 };
		}
	});

	GameObjectEntity* m = new EnemyEntity({ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f });
	m->AddPreUpdateListener(this, [m]() {
		m->color = { 255, 255, 255, 255 };
		m->rotation += Utils::GetDeltaTime();
		if (m->rotation >= 360.f) {
			m->rotation -= 360.f;
		}
	});
	m->AddUpdateListener(this, [m]() {
		if (m->pBody->is_colliding) {
			m->color = { 255, 0, 0, 0 };
		}
		s32 mouse_x, mouse_y;
		AEInputGetCursorPosition(&mouse_x, &mouse_y);
		AEVec2 mouse{ static_cast<f32>(mouse_x), static_cast<f32>(mouse_y) };
		AEVec2 mouse_world = Utils::Screen_To_World(mouse.x, mouse.y);

		if (Utils::OBBPoint(m, mouse_world) && AEInputCheckCurr(AEVK_LBUTTON)) {
			m->position = mouse_world;
		}
	});
	m->go_type = GameObjectEntity::KINEMATIC::STATIC;

	scene_entities.push_back(m);
	scene_entities.push_back(p);
	scene_entities.push_back(e);
	gameObjects.push_back(p);
	gameObjects.push_back(e);
	gameObjects.push_back(m);

	BaseEntity* w = new Weapon(AEVec2{ 0.f, 0.f }, p);
	scene_entities.push_back(w);
}

void GameScene::PreUpdate(const f32& dt) {
	BaseScene::PreUpdate(dt);
}

void GameScene::Update(const f32& dt) {
	BaseScene::Update(dt);

	if (AEInputCheckCurr(AEVK_LEFT)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		x -= dt * 100;
		AEGfxSetCamPosition(x, y);
	}
	if (AEInputCheckCurr(AEVK_RIGHT)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		x += dt * 100;
		AEGfxSetCamPosition(x, y);
	}
	if (AEInputCheckCurr(AEVK_UP)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		y += dt * 100;
		AEGfxSetCamPosition(x, y);
	}
	if (AEInputCheckCurr(AEVK_DOWN)) {
		f32 x, y;
		AEGfxGetCamPosition(&x, &y);
		y -= dt * 100;
		AEGfxSetCamPosition(x, y);
	}
	if (AEInputCheckTriggered(AEVK_G)) {
		DebugUtils::ToggleRender(!DebugUtils::IsRendering());
	}

	//Reset collision state to false;
	for (auto& go : gameObjects) { go->pBody->is_colliding = false; }

	// Collision detection 
	for (int i{}; i < gameObjects.size(); i++) {
		// Starts loop only from the next object
		for (int j{i + 1}; j < gameObjects.size(); j++) {
			GameObjectEntity* go = gameObjects[i];
			GameObjectEntity* go2 = gameObjects[j];
			//Checks if either go is inactive, if so, skip this check
			if (!go->isActive || !go2->isActive)
				continue;
			if (Utils::OBB(go, go2)) {
				//go->color = { 255, 0, 0, 0 };
				AEVec2 go1to2 = go2->position - go->position;
				go->pBody->is_colliding = true;
				go2->pBody->is_colliding = true;

				if (AEVec2DotProduct(&go->velocity, &go1to2) > 1)
				{
					//Velocity Trading

					AEVec2 tmp{ go->velocity };
					AEVec2 tmp2{ go2->velocity };
					f32 mass_total = go->pBody->mass + go2->pBody->mass;
					
					if (go->position.y > go2->position.y + go2->scale.y * 0.5f)
					{
						go->velocity.y = 0.0f;
						go2->velocity.y = (go2->velocity.y * (go2->pBody->mass - go->pBody->mass) + tmp.y * 2 * go->pBody->mass) / mass_total;
					}
					else
					{
						go2->velocity.y = 0.0f;
						go->velocity.y = (go->velocity.y * (go->pBody->mass - go2->pBody->mass) + tmp2.y * 2 * go2->pBody->mass) / mass_total;
						go->velocity.x = (go->velocity.x * (go->pBody->mass - go2->pBody->mass) + tmp2.x * 2 * go2->pBody->mass) / mass_total;
						go2->velocity.x = (go2->velocity.x * (go2->pBody->mass - go->pBody->mass) + tmp.x * 2 * go->pBody->mass) / mass_total;

					}




					/*AEVec2 go1_push = go->position - go2->position;
					AEVec2 go2_push = go2->position - go->position;
					Utils::SnapVectorToAxis(&go1_push, &go1_push);
					Utils::SnapVectorToAxis(&go2_push, &go2_push);
					go->velocity += go1_push;
					go2->velocity += go2_push;*/
					/*if (go1_push.y > go1_push.x || go2_push.y > go2_push.x)
					{
						if (go->position.y > go2->position.y)
						{
							go->velocity.y = 0;
						}
						else
						{
							go2->velocity.y = 0;
						}
					}
					else
					{
						go->velocity += go1_push;
						go2->velocity += go2_push;
					}*/
				}
			}
		}
	}

}

void GameScene::PostUpdate(const f32& dt) {
	BaseScene::PostUpdate(dt);
}

void GameScene::Render() {
	BaseScene::Render();
}

void GameScene::End() {
	BaseScene::End();
	InputEvent::Listeners -= OnGameExit;
	gameObjects.clear();
}