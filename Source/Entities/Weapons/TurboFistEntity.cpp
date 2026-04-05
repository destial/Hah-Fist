/*!
* @file TurboFistEntity.cpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @date 24th February 2026
* @course CSD1451
* @brief This source file defines the member functions of the 
* TurboFistWeapon which inherits the weapon class.
* This weapon channels and releases to launch the player forward, speed of travel
* determined by the duration of travel.
*/

#include "TurboFistEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../UI/Debug.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Enemies/EnemyEntity.hpp"
#include "../Projectiles/BaseProjectile.hpp"


TurboFistWeapon::TurboFistWeapon(AEVec2 pos, f32 og_mass) : WeaponEntity{ pos } {
	weapon_channels = true;
	image = AssetManager::GetTexture(ASSET_TURBOFIST_IMAGE);
	max_channel_time = 1.0f;
	cd_duration = 0.5f;
	damage = 1.0f;
	player_original_mass = og_mass;
}

TurboFistWeapon::~TurboFistWeapon() {} // Empty dtor

void TurboFistWeapon::Update(const f32& dt) {
	if (dt == 0)
		return;

	if (!is_active) 
		return;

	WeaponEntity::Update(dt);
	
	if (dash_timer > 0.0f) {
		dash_timer -= dt;
	}
	else if (player_entity->pBody->mass != player_original_mass) {
		player_entity->pBody->mass = player_original_mass;
		player_entity->friction_multiplier = 1.0;
	}
}

void TurboFistWeapon::Render() {
	WeaponEntity::Render();
	if (channelling) {
		AEVec2 normalizedDirection{ weapon_direction };
		AEVec2Normalize(&normalizedDirection, &normalizedDirection);
		DebugUtils::RenderLine(position, position + (position - player_entity->position) * GetCurrentAttackStrength() * 3.0);
	}
}

void TurboFistWeapon::OnCollide(GameObjectEntity* go) {
	if (go == player_entity) {
		return;
	}
	if (dash_timer > 0.0f) {
		if (go->entity_type == EntityType::ENEMY) {
			dynamic_cast<EnemyEntity*>(go)->OnHit();
			go->health -= damage * AEVec2Length(&player_entity->velocity);
			//go->velocity += player_entity->velocity * 1.25;
			dash_timer = 0;
		}
		else if (go->entity_type == EntityType::BREAKABLE_STATIC) {
			go->health -= damage * AEVec2Length(&player_entity->velocity);
			//go->velocity += player_entity->velocity * 1.25;
			dash_timer = 0;
		}
		else if (go->entity_type == EntityType::PROJECTILE) {
			BaseProjectile* e = dynamic_cast<BaseProjectile*>(go);
			
			//i think this is correct, not sure
			if (e->TryChangeOwnership(player_entity)) {
				go->velocity = player_entity->velocity * 1.5f;
			}
		}
	}
}

void TurboFistWeapon::Attack() {
	float attack_strength = GetCurrentAttackStrength();
	channel_timer = 0.0f;

	AEVec2 attack_direction = GetAttackDirection();

	dash_timer = max_dash_time;
	player_entity->invulnerability_duration = max_dash_time * 2;
	player_entity->velocity = attack_direction * 20 * attack_strength;
	player_entity->pBody->mass = 10.0f * attack_strength;
	player_entity->friction_multiplier = 0.25;

	AEAudioPlay(AssetManager::GetAudio(ASSET_TURBOFIST_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
}

f32 TurboFistWeapon::GetCurrentAttackStrength() const {
	return 1.f + 2.f * (static_cast<f32>(std::trunc(channel_timer * 10.f)) / (max_channel_time * 10.f));
}

void TurboFistWeapon::ResetWeapon() {
	WeaponEntity::ResetWeapon();
	dash_timer = 0.0f;
	dashing = false;
}