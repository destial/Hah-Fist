/*!
* @file PlayerEntity.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 12th January 2026
* @course CSD1451
* @brief This source file declares the PhysicsManager class and its member functions, which
* handles physics for all objects within the gameplay loop.
*/
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
/*!
* @brief Constructor function for a PlayerEntity. Initializes the basic data for the PlayerEntity to operate
* such as the world size, maximum entries per node for the QuadTree, and the zeroed vector of gameobjects.
* @param AEVec2 pos - position to spawn the entity.
*/
PlayerEntity::PlayerEntity(AEVec2 pos) : GameObjectEntity(pos) {
	int columns{ 18 };
	int rows{ 3 };

	sprite = AssetManager::GetSpriteSheet(ASSET_PLAYER_SPRITE, rows, columns);
	mesh = nullptr;
	animation_timer = 0.f;
	animation_frame = 1.f / static_cast<f32>(columns * rows);
	current_row = current_col = 0;
	scale = { 5.f * ((static_cast<f32>(sprite->image->width) / static_cast<f32>(columns)) / (sprite->image->height / static_cast<f32>(rows))) , 5.f };
	jump_height = 8.5f;
	jump_velocity = std::sqrtf(jump_height * 2.f * std::abs(pBody->gravity.y));
	speed = 10.f;
	health = 100.f;
	max_health = 100.f;
	time_elapsed_since_last_damage = PLAYER_CONTROL_LOCK_AFTER_HIT;

	entity_type = EntityType::PLAYER;
	layer = RenderLayer::PLAYER;
	go_type = PhysicsType::DYNAMIC;
}

/*!
* @brief Destructor function for a PlayerEntity.
* Removes its weapons from the game scene.
*/
PlayerEntity::~PlayerEntity() {
	std::printf("Called Player deconstructor\n");
	for (WeaponEntity* wp : weapons)
	{
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(wp);
	}
}

/*!
* @brief PreUpdate for a PlayerEntity.
* Resets color to white
* @param const f32& dt - delta time
*/
void PlayerEntity::PreUpdate(const f32& dt) {
	GameObjectEntity::PreUpdate(dt);
	color = { 255, 255, 255, 255 };
}

/*!
* @brief Update for a PlayerEntity.
* Resets lerps tint color based on damage and invulnerability
* @param const f32& dt - delta time
*/
void PlayerEntity::Update(const f32& dt) {
	GameObjectEntity::Update(dt);

	if ((time_elapsed_since_last_damage / PLAYER_CONTROL_LOCK_AFTER_HIT) <= 1.0)
	{
		this->color = Utils::Lerp(Color{ 255, 255, 128, 128 }, Color{ 255, 255, 255, 255 }, time_elapsed_since_last_damage / PLAYER_CONTROL_LOCK_AFTER_HIT);
	}
	else if (invulnerability_duration > 0)
	{
		this->color = Utils::Lerp(Color{ 255, 0, 128, 0 }, Color{ 255, 255, 255, 255 }, 20.f * invulnerability_duration);
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

/*!
* @brief PostUpdate for a PlayerEntity.
* Handles animations
* @param const f32& dt - delta time
*/
void PlayerEntity::PostUpdate(const f32& dt) {
	GameObjectEntity::PostUpdate(dt);
	current_row = 1;
	if (velocity.x != 0) {
		current_row = 0;
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
			current_row = 2;
		}
	}
	
	if ((animation_timer += dt) > animation_frame) {
		animation_timer = 0.f;
		if (++current_col >= 32) {
			current_col = 0;
		}
	}
}

/*!
* @brief Render for a PlayerEntity.
* Renders the player
*/
void PlayerEntity::Render() {
	sprite->Render(transform, color, current_row, current_col);
	GameObjectEntity::Render();
}

/*!
* @brief OnCollide override for the player, resolves damage, pushback and 
* invulnerability.
* @param GameObjectEntity* go - Object it is colliding with.
*/
void PlayerEntity::OnCollide(GameObjectEntity* go) {
	//f32 p_health = health;
	
	if (go->entity_type == EntityType::ENEMY) {
		if (invulnerability_duration > 0) { return; }
		time_elapsed_since_last_damage = 0.0f;
		invulnerability_duration = 0.75f;
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

/*!
* @brief Adds a given weapon to the player.
* @param WeaponEntity* weapon - weapon to add.
*/
void PlayerEntity::AddWeapon(WeaponEntity* weapon) {
	weapons.push_back(weapon);
}

/*!
* @brief Switches the active weapon.
* @param int index - index of the weapon.
*/
void PlayerEntity::SwitchWeapon(int index) {
	if (weapons.size() <= index) 
		return;

	for (int i{ 0 }; i < weapons.size(); ++i) {
		if (i != index) {
			weapons[i]->ResetWeapon();
		}
		weapons[i]->is_active = i == index;
	}
}

/*!
* @brief Gets the active weapon.
*/
WeaponEntity* PlayerEntity::CurrentWeapon() const {
	for (int i{ 0 }; i < weapons.size(); ++i) {
		if (weapons[i]->is_active) {
			return weapons[i];
		}
	}
	return nullptr;
}

/*!
* @brief Adds a coin to the player.
*/
void PlayerEntity::AddCoin() {
	coin_count++;
}

/*!
* @brief Gets the amount of coins the player has.

*/
int PlayerEntity::Coins() const {
	return coin_count;
}

f32 PlayerEntity::GetSpeed() const {
	return speed;
}

void PlayerEntity::Jump() {
	velocity.y += jump_velocity;
}

void PlayerEntity::Move(AEVec2 const& dir) {
	//f32 spd = velocity.y == 0 ? speed : speed * pBody->air_strength * 0.75f;
	if (!(dir.x < 0 && velocity.x < 0 || dir.x > 0 && velocity.x > 0)) {
		velocity.x = 0.f;
	}

	if (std::fabs(velocity.x) < speed) {
		velocity.x += dir.x * speed;
	}
}
