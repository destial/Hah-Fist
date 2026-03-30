#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/CameraManager.hpp"
#include "../Scenes/BaseScene.hpp"  
#include "../Managers/SceneManager.hpp"
#include <iostream>
#include <cstdio>
#include <cmath>

//For Debug Functions
#include "../Entities/StaticEntities/BossSpawnTriggerEntity.hpp"

Player::Player(AEVec2 pos) : GameObjectEntity(pos) {
	int columns{ 18 };
	int rows{ 3 };

	sprite = AssetManager::GetSpriteSheet(ASSET_PLAYER_SPRITE, rows, columns);
	mesh = nullptr;
	animationTimer = 0.f;
	animationFrame = 1.f / static_cast<f32>(columns * rows);
	currentRow = currentCol = 0;
	scale = { 5.f * ((static_cast<f32>(sprite->image->width) / static_cast<f32>(columns)) / (sprite->image->height / static_cast<f32>(rows))) , 5.f };
	jumpHeight = 8.5f;
	jumpVelocity = std::sqrtf(jumpHeight * 2.f * std::abs(pBody->gravity.y));
	speed = 10.f;
	health = 100.f;
	max_health = 100.f;
	timeElapsedSinceLastDamage = PLAYER_CONTROL_LOCK_AFTER_HIT;

	entity_type = EntityType::PLAYER;
	layer = RenderLayer::PLAYER;
	go_type = PhysicsType::DYNAMIC;
}

Player::~Player() {
	std::printf("Called Player deconstructor\n");
	for (Weapon* wp : weapons)
	{
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(wp);
	}
}

void Player::PreUpdate(const f32& dt) {
	GameObjectEntity::PreUpdate(dt);
	color = { 255, 255, 255, 255 };
}

void Player::Update(const f32& dt) {
	GameObjectEntity::Update(dt);

	if ((timeElapsedSinceLastDamage / PLAYER_CONTROL_LOCK_AFTER_HIT) <= 1.0)
	{
		this->color = Utils::Lerp(Color{ 255, 255, 128, 128 }, Color{ 255, 255, 255, 255 }, timeElapsedSinceLastDamage / PLAYER_CONTROL_LOCK_AFTER_HIT);
	}
	else if (invulnerabilityDuration > 0)
	{
		this->color = Utils::Lerp(Color{ 255, 0, 128, 0 }, Color{ 255, 255, 255, 255 }, 20.f * invulnerabilityDuration);
	}
	
	

	////Testing Shooting Function
	if (AEInputCheckCurr(AEVK_T)) {
		health = 1000.f;
		max_health = 1000.f;
	}
	if (AEInputCheckCurr(AEVK_Y)) {
		position = dynamic_cast<BossSpawnTriggerEntity*>(SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<BossSpawnTriggerEntity>())->position;
	}
}

void Player::PostUpdate(const f32& dt) {
	GameObjectEntity::PostUpdate(dt);
	currentRow = 1;
	if (velocity.x != 0) {
		currentRow = 0;
		if (velocity.x < 0) {
			if (this->scale.x > 0) {
				this->scale.x *= -1;
			}
		}
		else {
			if (this->scale.x < 0) {
				this->scale.x *= -1;
			}
		}
		if (AEVec2Length(&velocity) > 50.0f) {
			currentRow = 2;
		}
	}
	
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 32) {
			currentCol = 0;
		}
	}
}

void Player::Render() {
	sprite->Render(transform, color, currentRow, currentCol);
	GameObjectEntity::Render();
}

void Player::OnCollide(GameObjectEntity* go) {
	//f32 p_health = health;
	
	if (go->entity_type == EntityType::ENEMY) {
		if (invulnerabilityDuration > 0) { return; }
		timeElapsedSinceLastDamage = 0.0f;
		invulnerabilityDuration = 0.75f;
		AEVec2 push_velocity = position - go->position;
		AEVec2Normalize(&push_velocity, &push_velocity);
		velocity.x += push_velocity.x * 25.f;
		if (go->position.y <= position.y) {
			velocity.y += std::fabsf(push_velocity.y) * 25.f;
		}
		health -= go->damage;
		CurrentWeapon()->ResetWeapon();
		AEAudioPlay(AssetManager::GetAudio(ASSET_PLAYERHURT_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
	}
	GameObjectEntity::OnCollide(go);
}

void Player::AddWeapon(Weapon* weapon) {
	weapons.push_back(weapon);
}

void Player::SwitchWeapon(int index) {
	if (weapons.size() <= index) 
		return;

	for (int i{ 0 }; i < weapons.size(); ++i) {
		if (i != index) {
			weapons[i]->ResetWeapon();
		}
		weapons[i]->isActive = i == index;
	}
}

Weapon* Player::CurrentWeapon() const {
	for (int i{ 0 }; i < weapons.size(); ++i) {
		if (weapons[i]->isActive) {
			return weapons[i];
		}
	}
	return nullptr;
}

void Player::AddCoin() {
	coinCount++;
}

int Player::Coins() const {
	return coinCount;
}

f32 Player::GetSpeed() const {
	return speed;
}

void Player::Jump() {
	velocity.y += jumpVelocity;
}

void Player::Move(AEVec2 const& dir) {
	//f32 spd = velocity.y == 0 ? speed : speed * pBody->air_strength * 0.75f;
	if (!(dir.x < 0 && velocity.x < 0 || dir.x > 0 && velocity.x > 0)) {
		velocity.x = 0.f;
	}

	if (std::fabs(velocity.x) < speed) {
		velocity.x += dir.x * speed;
	}
}
