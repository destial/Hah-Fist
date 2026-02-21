#include "GameScene.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/EnemyEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../UI/Debug.hpp"
#include "AEMath.h"
#include <cstdio>
#include <string>

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, std::string str) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture("Assets/Icons/small_button_grey.png");
	b->color = { 255, 255, 255, 255 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->overlay_color = b->color;
	b->text = str;
	b->text_size = 7.5f;
	b->font = AssetManager::GetFontId("Assets/Jersey25-Regular.ttf");
	return b;
}

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, char ch) {
	return CreateHotKeyDisplay(pos, std::string{ ch });
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

	ButtonUI* s = new ButtonUI({ 3.f, Utils::GetWorldHeight() - .6f });
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

	ButtonUI* cam = new ButtonUI({ 3.f, Utils::GetWorldHeight() - 1.5f });
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

	ButtonUI* mw = new ButtonUI({ 3.f, Utils::GetWorldHeight() - 2.5f });
	mw->color = { 0, 0, 0, 0 };
	mw->overlay_color = mw->color;
	mw->scale.x = 5.5f;
	mw->text_size = 5.f;
	mw->text = "Mouse:";
	mw->text_alignment = BaseUI::TEXT_ALIGNMENT::LEFT_CORNER;
	mw->AddUpdateListener(this, [mw]() {
		AEVec2 mwp = Utils::GetMouseWorld(true);
		char b[128];
		sprintf_s(b, "Mouse:%.2f,%.2f", mwp.x, mwp.y);
		mw->text = std::string(b);
	});

	AddEntityToScene(s);
	AddEntityToScene(cam);
	AddEntityToScene(mw);

	ButtonUI* wk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 1.f }, 'W');
	ButtonUI* ak = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 3.f, Utils::GetWorldHeight() - 2.f }, 'A');
	ButtonUI* dk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 1.f, Utils::GetWorldHeight() - 2.f }, 'D');
	ButtonUI* sk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 2.f }, "Sp");

	AddEntityToScene(wk);
	AddEntityToScene(ak);
	AddEntityToScene(dk);
	AddEntityToScene(sk);

	GameObjectEntity* p = new Player({ 1.f, 1.f });
	std::printf("Player mass :%f\n", p->pBody->mass);
	GameObjectEntity* wall = new GameObjectEntity({ 20.f, 7.f });
	wall->go_type = GameObjectEntity::KINEMATIC::STATIC;
	wall->mesh = MeshRenderer::GetCenterRectMesh();
	wall->layer = 3;
	wall->scale = { 30.f, 1.f };
	AddEntityToScene(wall);

	AddEntityToScene(p);

	for (int i{}; i < 2; i++)
	{
		GameObjectEntity* e = new EnemyEntity({ 9.f + float(i) * 5.0f, 2.5f});
		e->pBody->mass = 40.0f;
		AddEntityToScene(e);
	}

	BaseEntity* w = new Weapon(AEVec2{ 0.f, 0.f }, p);
	AddEntityToScene(w);

	for (GameObjectEntity* go : physicsManager->gameObjects) {
		go->AddUpdateListener(this, [go]() {
			AEVec2 mw = Utils::GetMouseWorld(true);
			if (AEInputCheckCurr(AEVK_LBUTTON) && Utils::OBBPoint(go, mw)) {
				go->position = mw;
			}
		});
	}

	InputEvent::Listeners += {this, [this](const InputEvent* ev) {
		if (ev->IsKeyTriggered(AEVK_1)) {
			GameObjectEntity* go = new GameObjectEntity(Utils::GetMouseWorld(true));
			go->mesh = MeshRenderer::GetCenterRectMesh();
			go->go_type = GameObjectEntity::KINEMATIC::STATIC;
			AddEntityToScene(go);
		}
	}};
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
}

void GameScene::PostUpdate(const f32& dt) {
	BaseScene::PostUpdate(dt);
}

void GameScene::Render() {
	BaseScene::Render();
}

void GameScene::End() {
	BaseScene::End();
	InputEvent::Listeners -= this;
}