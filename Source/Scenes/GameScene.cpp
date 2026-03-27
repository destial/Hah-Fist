/*!
* @file GameScene.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 7 January 2026
* @course CSD1451
* @brief Definition file for a base scene that will be inherited for all scenes
*/

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
#include "../Entities/DropEntities/CoinEntity.hpp"
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
#include <iostream>
#include <sstream>
#include <cmath>
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

static void OnGameExit(const InputEvent* ev) {
	if (ev->IsKeyTriggered(AEVK_ESCAPE)) {
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}

GameScene::GameScene() : BaseScene(), game_timer{ 0 }, game_state{ GameState::INIT } {} // Empty ctor body

GameScene::~GameScene() {} // Empty dtor

void GameScene::Init() {
	staticEntities.reserve(SIZE_TO_RESERVE); // Reserves StaticEntities capacity for vector arr
	camManager->Init();
	game_timer = 0.f;

	// Add input listener when ESC to pressed to exit back to main menu
	InputEvent::Listeners += {this, OnGameExit};

	// Initialize background image
	ImageUI* bgd = new ImageUI{ ASSET_BACKGROUND_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f} };
	bgd->layer = BaseEntity::RenderLayer::BACKGROUND;
	bgd->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	AddEntityToScene(bgd);

	// Load level based on level file
	std::string filename = "Assets/level_";
	filename += std::string{ static_cast<char>('0' + LevelManager::GetLevel()) };
	filename += ".dat";
	std::vector<Serialization::SerializedEntity> ens = Serialization::LoadFromFile(filename.c_str());

	// Unserialize all entity data from level file
	Player* player = nullptr;
	if (!ens.empty()) {
		for (Serialization::SerializedEntity const& sen : ens) {
			BaseEntity* en = Serialization::Unserialize(sen);
			if (en) { // If it's a valid entity
				AddEntityToScene(en);
				if (Player* p = dynamic_cast<Player*>(en)) {
					player = p; // Set the player pointer when loaded (only 1 should exist) 
				}
			}
		}
		std::printf("Loaded from file");
	}
	else { // No level file, create generic base level
		GameObjectEntity* wall = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_WALL, AEVec2{ 1.f, 1.f });
		wall->mesh = MeshRenderer::GetCenterRectMesh();
		wall->scale = { 30.f, 1.f };
		AddEntityToScene(wall);	
	}

	if (player == nullptr) {
		player = new Player({ 1.f, 5.f });
		AddEntityToScene(player);
	}

	// Lock weapons based on level
	Weapon* w = new TurboFistWeapon(AEVec2{ 0.f, 0.f }, player->pBody->mass);
	AddEntityToScene(w);
	player->AddWeapon(w);
	if (LevelManager::GetLevel() > 0) {
		Weapon* w2 = new GrappleFistWeapon(AEVec2{ 0.f, 0.f });
		AddEntityToScene(w2);
		player->AddWeapon(w2);
	}
	if (LevelManager::GetLevel() > 1) {
		Weapon* w3 = new FingerGunWeapon(AEVec2{ 0.f, 0.f });
		AddEntityToScene(w3);
		player->AddWeapon(w3);
	}
	player->SwitchWeapon(0);

	// Initialize weapon UI elements, the current charge / cooldown of the weapon
	BarUI* power = new BarUI{ AEVec2{ 0.f, 0.f } };
	power->scale = { 2.f, .25f };
	power->text = "";
	power->SetInteractive(false);
	power->SetValue(0.5f);
	power->layer = BaseUI::RenderLayer::PLAYER;
	power->text_size = 7.f;
	AddEntityToScene(power);
	power->AddUpdateListener(this, [power, player](const f32& dt) {
		// Update UI based on current weapon position and value
		Weapon* current = player->CurrentWeapon();
		if (current == nullptr) 
			return;

		// Use cooldown timer as priority
		if (current->GetCooldownTimer() > 0) {
			power->SetValue(current->GetCooldownTimer() / current->GetCooldownDuration());
			power->overlay_color = { 255, 128, 128, 128 };
		}
		else {
			power->SetValue(current->GetChannelTimer() / current->GetMaxChannelTime());
			power->overlay_color = { 255, 255, 0, 0 };
		}

		// Always render below the weapon
		power->position = current->position;
		power->position.y -= std::abs(current->scale.y) * 0.65f;
	});

	player->AddUpdateListener(this, [player](const f32& dt) {
		if (player->timeElapsedSinceLastDamage > PLAYER_CONTROL_LOCK_AFTER_HIT) {
			AEVec2 dir{};
			if (AEInputCheckCurr(AEVK_A)) {
				dir += { -1.f, 0.f };
			}
			if (AEInputCheckCurr(AEVK_D)) {
				dir += { 1.f, 0.f };
			}
			if (dir.x || dir.y) {
				AEVec2Normalize(&dir, &dir);
			}

			if (dir.x && player->pBody->is_standing_above) {
				player->Move(dir);
			}
		}

		if (AEInputCheckCurr(AEVK_1)) {
			player->SwitchWeapon(0);
		}
		else if (AEInputCheckCurr(AEVK_2)) {
			player->SwitchWeapon(1);
		}
		else if (AEInputCheckCurr(AEVK_3)) {
			player->SwitchWeapon(2);
		}

		if (AEInputCheckCurr(AEVK_SPACE) && player->pBody->is_standing_above) {
			player->Jump();
		}
	});

	player->AddUpdateListener(AssetManager::GetInstance(), [this, player](const f32& dt) {
		if (BossEntity * e = dynamic_cast<BossEntity*>(GetFirstEntityOfType<BossEntity>())) {
			if (e->GetBossActivated()) {
				camManager->SetTarget(Utils::WorldToScreen(e->GetBossRoomCenter().x, e->GetBossRoomCenter().y).x, 0);
				return;
			}
		}
		if (game_state == GameState::PLAYING)
			camManager->SetPosition(Utils::WorldToScreen(player->position.x, player->position.y).x, 0);
	});

	ImageUI* hud = new ImageUI{ ASSET_HUD_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() - 1.7f} };
	hud->scale = { Utils::GetWorldWidth() - 5.f, 3.f };
	hud->color.a = 210;
	hud->SetInteractive(false);
	AddEntityToScene(hud);

	ButtonUI* lb = CreateHotKeyDisplay({ Utils::GetWorldWidth() - 4.5f, Utils::GetWorldHeight() - 1.f }, "LB");
	lb->AddUpdateListener(lb, [lb](const f32& dt) {
		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			lb->color = { 255, 128, 128, 128 };
		}
	});
	ButtonUI* ak = CreateHotKeyDisplay({ Utils::GetWorldWidth() - 5.5f, Utils::GetWorldHeight() - 2.f }, 'A');
	ak->AddUpdateListener(ak, [ak](const f32& dt) {
		if (AEInputCheckCurr(AEVK_A)) {
			ak->color = { 255, 128, 128, 128 };
		}
	});
	ButtonUI* dk = CreateHotKeyDisplay({ Utils::GetWorldWidth() - 3.5f, Utils::GetWorldHeight() - 2.f }, 'D');
	dk->AddUpdateListener(dk, [dk](const f32& dt) {
		if (AEInputCheckCurr(AEVK_D)) {
			dk->color = { 255, 128, 128, 128 };
		}
	});
	ButtonUI* sk = CreateHotKeyDisplay({ Utils::GetWorldWidth() - 4.5f, Utils::GetWorldHeight() - 2.f }, "Sp");
	sk->AddUpdateListener(sk, [sk](const f32& dt) {
		if (AEInputCheckCurr(AEVK_SPACE)) {
			sk->color = { 255, 128, 128, 128 };
		}
	});

	AddEntityToScene(lb);
	AddEntityToScene(ak);
	AddEntityToScene(dk);
	AddEntityToScene(sk);

	BarUI* player_health = new BarUI( {Utils::GetWorldWidth() * 0.2f, Utils::GetWorldHeight() - 1.5f} );
	player_health->text_alignment = BaseUI::TEXT_ALIGNMENT::LEFT_CORNER;
	player_health->text_size = 5.f;
	player_health->scale = { 12.f, 1.5f };
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

	BarUI* weaponhud = new BarUI{ {Utils::GetWorldWidth() * 0.49f, Utils::GetWorldHeight() - 1.5f} };
	weaponhud->text_alignment = BaseUI::TEXT_ALIGNMENT::LEFT_CORNER;
	weaponhud->text_size = 5.f;
	weaponhud->scale = { 12.f, 1.5f };
	weaponhud->text = "";
	weaponhud->SetInteractive(false);
	weaponhud->layer = BaseUI::RenderLayer::UI;
	AddEntityToScene(weaponhud);
	weaponhud->AddUpdateListener(this, [weaponhud, player](const f32& dt) {
		Weapon* current = player->CurrentWeapon();
		if (current == nullptr) {
			weaponhud->text = "";
			return;
		}
		weaponhud->overlay_color = { 255, 255, 0, 0 };
		if (current->GetCooldownTimer() > 0) {
			weaponhud->SetValue(current->GetCooldownTimer() / current->GetCooldownDuration());
			weaponhud->overlay_color = { 255, 128, 128, 128 };
			char cooldown[64];
			sprintf_s(cooldown, 64, "Cooldown: %0.2f%%", (current->GetCooldownTimer() / current->GetCooldownDuration()) * 100.f);
			weaponhud->text = cooldown;
		}
		else {
			weaponhud->SetValue(current->GetChannelTimer() / current->GetMaxChannelTime());
			char charge[64];
			sprintf_s(charge, 64, "Charge: %0.2f%%", (current->GetChannelTimer() / current->GetMaxChannelTime()) * 100.f);
			weaponhud->text = charge;
		}
	});

	ImageUI* coins = new ImageUI{ ASSET_SMALLBUTTON_IMAGE, {Utils::GetWorldWidth() * 0.69f, Utils::GetWorldHeight() - 1.5f} };
	coins->text_size = 7.5f;
	coins->scale = {5.f, 2.f};
	coins->AddUpdateListener(this, [coins, player](const f32& dt) {
		char collected[64];
		sprintf_s(collected, 64, "Coins: %d", player->Coins());
		coins->text = collected;
	});
	AddEntityToScene(coins);

	ImageUI* time = new ImageUI{ ASSET_SMALLBUTTON_IMAGE, {Utils::GetWorldWidth() * 0.81f, Utils::GetWorldHeight() - 1.5f} };
	time->text_size = 7.5f;
	time->scale = { 5.f, 2.f };
	time->AddUpdateListener(this, [time, this](const f32& dt) {
		char timer[64];
		sprintf_s(timer, 64, "Timer: %0.2f", game_timer);
		time->text = timer;
	});
	AddEntityToScene(time);

	LevelManager::LoadTutorial(this);

	Game::SetBackgroundColor({ 1.f, 0.3f, 0.3f, 0.3f });
	game_state = GameState::PLAYING;
}

void GameScene::Update(const f32& dt) {
	staticEntities = SceneManager::GetInstance()->GetCurrentScene()->GetBaseEntitiesOfType<StaticEntity>();
	BaseScene::Update(dt);
	staticEntities.clear();
	if (game_state == GameState::PLAYING)
		game_timer += dt;

	if (AEInputCheckTriggered(AEVK_J)) {
		Win();
	}
}

void GameScene::PostUpdate(const f32& dt) {
	BaseScene::PostUpdate(dt);

	if (game_state == GameState::LOST) {
		End(); // restart level (todo: lose screen)
		Init();
	}

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
	LevelManager::UnlockLevel(LevelManager::GetLevel() + 1); // unlock next level

	LevelManager::SavePlayerData(); // save data

	Player* p = GetFirstEntityOfType<Player>();
	if (!p)
		return;

	ImageUI* bgd = new ImageUI{ ASSET_HUD_IMAGE, {Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f} };
	bgd->scale = { Utils::GetWorldWidth() * 0.3f, Utils::GetWorldHeight() * 0.9f };
	AddEntityToScene(bgd);

	ButtonUI* toptext = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.7f } };
	toptext->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	toptext->text_size = 10.f;
	toptext->color.a = 0;
	int score = static_cast<int>((p->Coins() / game_timer) * 100.f);
	std::ostringstream oss;
	oss.precision(2);
	oss << "You beat this level!   \nScore: " << score << "  \nGame Time: " << game_timer << "s   ";
	toptext->text = oss.str();
	toptext->SetInteractive(false);
	AddEntityToScene(toptext);

	ButtonUI* advance = new ButtonUI{{ Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }};
	advance->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	advance->text_size = 10.f;
	advance->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	advance->text = "Next Level  ";
	advance->AddClickListener([this](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			LevelManager::SetLevel(LevelManager::GetLevel() + 1);
			Lose();
		}
	});
	AddEntityToScene(advance);

	ButtonUI* back = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.35f } };
	back->scale = { Utils::GetWorldWidth() * 0.21f, Utils::GetWorldHeight() * 0.1f };
	back->text_size = 10.f;
	back->image = AssetManager::GetTexture(ASSET_SMALLBUTTON_IMAGE);
	back->text = "Back to Menu   ";
	back->AddClickListener([this](BaseUI::MouseButton b) {
		if (b & BaseUI::MouseButton::LEFT) {
			SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
		}
	});
	AddEntityToScene(back);

	p->RemoveUpdateListener(this);
}

void GameScene::Lose() {
	game_state = GameState::LOST;
}

std::vector<BaseEntity*>& GameScene::GetStaticEntities() {
	return staticEntities;
}
