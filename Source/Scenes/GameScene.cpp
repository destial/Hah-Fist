#include "GameScene.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SerializationManager.hpp"
#include "../Managers/LevelManager.hpp"
#include "../Entities/GameObjectEntity.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/Weapons/TurboFistEntity.hpp"
#include "../Entities/Weapons/GrappleFistEntity.hpp"
#include "../Entities/Weapons/FingerGunEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/BarUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../UI/Debug.hpp"
#include "AEMath.h"
#include <cstdio>
#include <string>

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, std::string str) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	b->color = { 255, 255, 255, 255 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->overlay_color = b->color;
	b->text = str;
	b->text_size = 7.5f;
	b->font = AssetManager::GetFontId(ASSET_DEFAULT_FONT);
	return b;
}

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, char ch) {
	return CreateHotKeyDisplay(pos, std::string{ ch });
}

GameScene::GameScene() : BaseScene(), game_timer{ 0 } {}

GameScene::~GameScene() {}

static void OnGameExit(const InputEvent* ev) {
	if (ev->IsKeyTriggered(AEVK_ESCAPE)) {
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}

void GameScene::Init() {
	camManager->Init();
	game_timer = 0.f;
	InputEvent::Listeners += {this, OnGameExit};

	ButtonUI* wk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 1.f }, "LB");
	ButtonUI* ak = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 3.f, Utils::GetWorldHeight() - 2.f }, 'A');
	ButtonUI* dk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 1.f, Utils::GetWorldHeight() - 2.f }, 'D');
	ButtonUI* sk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 2.f }, "Sp");

	AddEntityToScene(wk);
	AddEntityToScene(ak);
	AddEntityToScene(dk);
	AddEntityToScene(sk);

	std::string filename = "Assets/level_";
	filename += std::string{ static_cast<char>('0' + LevelManager::GetLevel()) };
	filename += ".dat";
	std::vector<Serialization::SerializedEntity> ens = Serialization::LoadFromFile(filename.c_str());

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
	Weapon* w = new TurboFistWeapon(AEVec2{ 0.f, 0.f }, player);
	AddEntityToScene(w);
	Weapon* w2 = new GrappleFistWeapon(AEVec2{ 0.f, 0.f }, player);
	AddEntityToScene(w2);
	Weapon* w3= new FingerGunWeapon(AEVec2{ 0.f, 0.f }, player);
	AddEntityToScene(w3);
	player->AddWeapon(w);
	player->AddWeapon(w2);
	player->AddWeapon(w3);

	player->SwitchWeapon(0);

	player->AddPostUpdateListener(this, [this, player](const f32& dt) {
		if (!player->isActive) {
			Lose();
		}
	});

	BarUI* power = new BarUI{ AEVec2{ 0.f, 0.f } };
	power->scale = { 2.f, .25f };
	power->text = "";
	power->SetInteractive(false);
	power->SetValue(0.5f);
	power->layer = BaseUI::RenderLayer::PLAYER;
	power->text_size = 7.f;
	AddEntityToScene(power);
	power->AddUpdateListener(this, [power, player](const f32& dt) {
		Weapon* current = player->CurrentWeapon();
		power->overlay_color = { 255, 255, 0, 0 };
		if (current->GetCooldownTimer() > 0) {
			power->SetValue(current->GetCooldownTimer() / current->GetCooldownDuration());
			power->overlay_color = { 255, 128, 128, 128 };
		}
		else {
			power->SetValue(current->GetChannelTimer() / current->GetMaxChannelTime());
		}
		power->position = current->position;
		power->position.y -= std::abs(current->scale.y) * 0.65f;
	});

	Game::SetBackgroundColor(Color{ 1.f, 0.3f, 0.3f, 0.3f });
}

void GameScene::Update(const f32& dt) {
	BaseScene::Update(dt);
	game_timer += dt;
}

void GameScene::End() {
	BaseScene::End();
	InputEvent::Listeners -= this;
}

void GameScene::Win() {
	LevelManager::SetLevelTime(LevelManager::GetLevel(), game_timer); // set this level time
	LevelManager::SetLevelTime(LevelManager::GetLevel() + 1, 0); // unlock next level
	LevelManager::SavePlayerData(); // save data
}

void GameScene::Lose() {
	End(); // restart level (todo: lose screen)
	Init();
}
