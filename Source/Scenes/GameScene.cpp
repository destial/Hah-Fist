#include "GameScene.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SerializationManager.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/Weapons/TurboFistEntity.hpp"
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
	AEGfxSetCamPosition(0.f, 0.f); // CameraEntity
	InputEvent::Listeners += OnGameExit;

	ButtonUI* wk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 1.f }, 'W');
	ButtonUI* ak = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 3.f, Utils::GetWorldHeight() - 2.f }, 'A');
	ButtonUI* dk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 1.f, Utils::GetWorldHeight() - 2.f }, 'D');
	ButtonUI* sk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 2.f }, "Sp");

	AddEntityToScene(wk);
	AddEntityToScene(ak);
	AddEntityToScene(dk);
	AddEntityToScene(sk);

	std::vector<Serialization::SerializedEntity> ens = Serialization::LoadFromFile("Assets/level_0.dat");

	Player* player = nullptr;
	if (!ens.empty()) {
		for (Serialization::SerializedEntity const& sen : ens) {
			BaseEntity* en = Serialization::Unserialize(sen);
			if (en) {
				AddEntityToScene(en);
				if (Player* p = dynamic_cast<Player*>(en)) {
					player = p;
				}
			}
		}
		std::printf("Loaded from file");
	}
	else {
		player = new Player({ 1.f, 1.f });
		std::printf("Player mass :%f\n", player->pBody->mass);
		GameObjectEntity* wall = new GameObjectEntity({ 20.f, 7.f });
		wall->go_type = GameObjectEntity::PhysicsType::STATIC;
		wall->mesh = MeshRenderer::GetCenterRectMesh();
		wall->scale = { 30.f, 1.f };
		AddEntityToScene(wall);
		AddEntityToScene(player);
		for (int i{}; i < 2; i++)
		{
			GameObjectEntity* e = new EnemyEntity({ 9.f + float(i) * 5.0f, 2.5f });
			e->pBody->mass = 1.0f;
			AddEntityToScene(e);
		}
		GameObjectEntity* trooper = new TrooperEntity{ {20.f, 12.f} };
		AddEntityToScene(trooper);
	}
	BaseEntity* w = new TurboFistWeapon(AEVec2{ 0.f, 0.f }, player);
	AddEntityToScene(w);
}

void GameScene::PreUpdate(const f32& dt) {
	BaseScene::PreUpdate(dt);
}

void GameScene::Update(const f32& dt) {
	BaseScene::Update(dt);
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