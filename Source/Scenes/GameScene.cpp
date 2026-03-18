#include "GameScene.hpp"
#include "../Managers/SceneManager.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SerializationManager.hpp"
#include "../Managers/LevelManager.hpp"
#include "../Entities/GameObjectEntity.hpp"
#include "../Entities/StaticEntity.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/Enemies/TitanEntity.hpp"
#include "../Entities/Enemies/PayloadEntity.hpp"
#include "../Entities/Enemies/IronsideEntity.hpp"
#include "../Entities/Weapons/TurboFistEntity.hpp"
#include "../Entities/Weapons/GrappleFistEntity.hpp"
#include "../Entities/Weapons/FingerGunEntity.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/Constant.hpp"
#include "../UI/ButtonUI.hpp"
#include "../UI/BarUI.hpp"
#include "../UI/ImageUI.hpp"
#include "../UI/CircleButtonUI.hpp"
#include "../UI/Debug.hpp"
#include "AEMath.h"
#include <cstdio>
#include <string>
constexpr int SIZE_TO_RESERVE{ 50 }; // Reserves this amt of space for vector arr


std::vector<BaseEntity*> GameScene::staticEntities;

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, std::string str) {
	ButtonUI* b = new ButtonUI(pos);
	b->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	b->SetInteractive(false);
	b->color = { 255, 255, 255, 255 };
	b->overlay_text_color = { 255, 255, 255, 255 };
	b->overlay_color = b->color;
	b->text = str;
	b->text_size = 7.5f;
	b->font = AssetManager::GetFontId(ASSET_DEFAULT_FONT);
	b->AddPreUpdateListener(b, [b](const f32& dt) {
		b->color = { 255, 255, 255, 255 };
	});
	return b;
}

static ButtonUI* CreateHotKeyDisplay(AEVec2 pos, char ch) {
	return CreateHotKeyDisplay(pos, std::string{ ch });
}

GameScene::GameScene() : BaseScene(), game_timer{ 0 }, game_state{ GameState::INIT } {}

GameScene::~GameScene() {}

static void OnGameExit(const InputEvent* ev) {
	if (ev->IsKeyTriggered(AEVK_ESCAPE)) {
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}

void GameScene::Init() {
	staticEntities.reserve(SIZE_TO_RESERVE); // Reserves StaticEntities capacity for vector arr
	camManager->Init();
	game_timer = 0.f;
	InputEvent::Listeners += {this, OnGameExit};

	ImageUI* bgd = new ImageUI{ ASSET_BACKGROUND_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f} };
	bgd->layer = BaseEntity::RenderLayer::BACKGROUND;
	bgd->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	AddEntityToScene(bgd);

	ButtonUI* lb = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 1.f }, "LB");
	lb->AddUpdateListener(lb, [lb](const f32& dt) {
		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			lb->color = { 255, 128, 128, 128 };
		}
	});
	ButtonUI* ak = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 3.f, Utils::GetWorldHeight() - 2.f }, 'A');
	ak->AddUpdateListener(ak, [ak](const f32& dt) {
		if (AEInputCheckCurr(AEVK_A)) {
			ak->color = { 255, 128, 128, 128 };
		}
	});
	ButtonUI* dk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 1.f, Utils::GetWorldHeight() - 2.f }, 'D');
	dk->AddUpdateListener(dk, [dk](const f32& dt) {
		if (AEInputCheckCurr(AEVK_D)) {
			dk->color = { 255, 128, 128, 128 };
		}
	});
	ButtonUI* sk = CreateHotKeyDisplay(AEVec2{ Utils::GetWorldWidth() - 2.f, Utils::GetWorldHeight() - 2.f }, "Sp");
	sk->AddUpdateListener(sk, [sk](const f32& dt) {
		if (AEInputCheckCurr(AEVK_SPACE)) {
			sk->color = { 255, 128, 128, 128 };
		}
	});

	AddEntityToScene(lb);
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

	// lock weapons based on level
	Weapon* w = new TurboFistWeapon(AEVec2{ 0.f, 0.f }, player);
	AddEntityToScene(w);
	player->AddWeapon(w);
	if (LevelManager::GetLevel() > 0) {
		Weapon* w2 = new GrappleFistWeapon(AEVec2{ 0.f, 0.f }, player);
		AddEntityToScene(w2);
		player->AddWeapon(w2);
	}
	if (LevelManager::GetLevel() > 1) {
		Weapon* w3 = new FingerGunWeapon(AEVec2{ 0.f, 0.f }, player);
		AddEntityToScene(w3);
		player->AddWeapon(w3);
	}

	player->SwitchWeapon(0);

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
		if (current == nullptr) return;
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

	player->AddUpdateListener(AssetManager::GetInstance(), [this, player](const f32& dt) {
		
		if (LevelManager::GetLevel() == 0)
		{
			if (dynamic_cast<TitanEntity*>(GetFirstEntityOfType<TitanEntity>()))
			{
				TitanEntity* e = dynamic_cast<TitanEntity*>(GetFirstEntityOfType<TitanEntity>());
				if (e->GetBossActivated())
				{
					camManager->SetTarget(Utils::WorldToScreen(e->GetBossRoomCenter().x, e->GetBossRoomCenter().y).x, 0);
				}
				else
				{
					camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
				}
			}
			else
			{
				camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
			}
			
		}
		else if (LevelManager::GetLevel() == 1)
		{
			if (dynamic_cast<PayloadEntity*>(GetFirstEntityOfType<PayloadEntity>()))
			{
				PayloadEntity* e = dynamic_cast<PayloadEntity*>(GetFirstEntityOfType<PayloadEntity>());
				if (e->GetBossActivated())
				{
					camManager->SetTarget(Utils::WorldToScreen(e->GetBossRoomCenter().x, e->GetBossRoomCenter().y).x, 0);
				}
				else
				{
					camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
				}
			}
			else
			{
				camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
			}
		}
		else if (LevelManager::GetLevel() == 2)
		{
			if (dynamic_cast<IronsideEntity*>(GetFirstEntityOfType<IronsideEntity>()))
			{
				IronsideEntity* e = dynamic_cast<IronsideEntity*>(GetFirstEntityOfType<IronsideEntity>());
				if (e->GetBossActivated())
				{
					camManager->SetTarget(Utils::WorldToScreen(e->GetBossRoomCenter().x, e->GetBossRoomCenter().y).x, 0);
				}
				else
				{
					camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
				}
			}
			else
			{
				camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
			}
		}
	});

	BarUI* player_health = new BarUI({Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() - 1.f});
	player_health->text_alignment = BaseUI::TEXT_ALIGNMENT::LEFT_CORNER;
	player_health->text_size = 5.f;
	player_health->scale = { 20.f, 1.f };
	player_health->overlay_color = { 255, 64, 255, 64 };
	player_health->color = { 255, 255, 64, 64 };
	player_health->layer = BaseUI::RenderLayer::UI;
	player_health->SetInteractive(false);
	player_health->AddUpdateListener(this, [player, player_health](const f32& dt) {
		if (player == nullptr) {
			return;
		}
		player_health->SetValue(player->health / player->max_health);
		char health[64];
		sprintf_s(health, 64, "Health: %0.2f / %0.2f", player->health, player->max_health);
		player_health->text = health;
	});
	AddEntityToScene(player_health);

	Game::SetBackgroundColor(Color{ 1.f, 0.3f, 0.3f, 0.3f });
	game_state = GameState::PLAYING;
}

void GameScene::Update(const f32& dt) {
	staticEntities = SceneManager::GetInstance()->GetCurrentScene()->GetBaseEntitiesOfType<StaticEntity>();
	BaseScene::Update(dt);
	staticEntities.clear();
	game_timer += dt;
}

void GameScene::PostUpdate(const f32& dt) {
	BaseScene::PostUpdate(dt);

	Player* player = GetFirstEntityOfType<Player>();
	if (player == nullptr || player->health <= 0.f)
		Lose();
}

void GameScene::End() {
	BaseScene::End();
	InputEvent::Listeners -= this;
}

void GameScene::Win() {
	game_state = GameState::WON;
	LevelManager::SetLevelTime(LevelManager::GetLevel(), game_timer); // set this level time
	if (!LevelManager::GetUnlockedLvls().count(LevelManager::GetLevel() + 1)) // check if next level is not unlocked
		LevelManager::UnlockLevel(LevelManager::GetLevel() + 1); // unlock next level
	LevelManager::SavePlayerData(); // save data
}

void GameScene::Lose() {
	game_state = GameState::LOST;
	End(); // restart level (todo: lose screen)
	Init();
}

std::vector<BaseEntity*>& GameScene::GetStaticEntities()
{
	return staticEntities;
}
