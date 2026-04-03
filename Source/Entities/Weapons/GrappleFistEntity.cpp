/*!
* @file GrappleFistWeapon.hpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @date 8th March 2026
* @course CSD1451
* @brief This source file declares the GrappleFistWeapon class which inherits from the weapon class.
* This weapon grapples the player towards structures and pulls entities towards the player.
*/
#include "GrappleFistEntity.hpp"
#include "../Projectiles/BaseProjectile.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"


GrappleFistWeapon::GrappleFistWeapon(AEVec2 pos) : WeaponEntity{ pos }, snapshotted_attack_direction{ 0 } {
	weapon_channels = false;
	image = AssetManager::GetTexture(ASSET_GRAPPLEFIST_IMAGE);
	cd_duration = 1.5f;
	damage = 50.f;
}

GrappleFistWeapon::~GrappleFistWeapon() {} // Empty dtor

void GrappleFistWeapon::PreUpdate(const f32& dt) {
	if (dt == 0)
		return;

	if (grapple_state == INACTIVE) {
		WeaponEntity::PreUpdate(dt);
	}
}

void GrappleFistWeapon::Update(const f32& dt) {
	if (dt == 0)
		return;

	if (!is_active) {
		return;
	}
	WeaponEntity::Update(dt);
}

void GrappleFistWeapon::PostUpdate(const f32& dt) {
	if (dt == 0)
		return;

	if (!is_active) {
		return;
	}
	WeaponEntity::PostUpdate(dt);
	AEVec2 travel_direction = position - player_entity->position;
	if (grapple_state == SHOOTING) {
		travel_duration += dt;
		position += snapshotted_attack_direction * dt * grapple_speed_mult;
		if (travel_duration >= 1.0f || AEVec2Length(&travel_direction) >= 20.0f) {
			travel_duration = 0.0f;
			grapple_state = RETURNING;
		}
		return;
	}
	else if (grapple_state == RETURNING) {
		if (AEVec2Length(&travel_direction) <= 5.0f) {
			grapple_state = INACTIVE;
		}
		AEVec2Normalize(&travel_direction, &travel_direction);
		position -= travel_direction * dt * grapple_speed_mult;
		return;
	}
	else if (grappled_object == nullptr || !grappled_object->is_active) {
		grappled_object = nullptr;
		grapple_state = INACTIVE;
	}
	else if (grappled_object != nullptr) {
		if (grapple_state == HOOKING_OBJECT_TO_PLAYER || grapple_state == HOOKING_PLAYER_TO_STATIC_OBJECT) {
			if (AEVec2Length(&travel_direction) <= 5.0f) {
				grappled_object = nullptr;
				grapple_state = INACTIVE;
				return;
			}
			AEVec2Normalize(&travel_direction, &travel_direction);
			if (grapple_state == HOOKING_OBJECT_TO_PLAYER) {
				position -= travel_direction * dt * grapple_speed_mult;
				grappled_object->position = position;
				grappled_object->health -= damage * dt;
			}
			else if (grapple_state == HOOKING_PLAYER_TO_STATIC_OBJECT) {
				player_entity->velocity = travel_direction * grapple_speed_mult;
			}
		}
		else if (grapple_state == HOOKING_PLAYER_TO_DYNAMIC_OBJECT) {
			player_entity->velocity = grappled_object->velocity;
			position = grappled_object->position;
		}
		return;
	}
}

void GrappleFistWeapon::OnCollide(GameObjectEntity* go) {
	if (go->go_type == PhysicsType::TRIGGER) {
		return;
	}
	if (go == player_entity) {
		return;
	}
	if (grapple_state == SHOOTING || grapple_state == RETURNING) {
		grappled_object = go;
		if (go->go_type == PhysicsType::DYNAMIC) {
			if (go->entity_type == EntityType::PROJECTILE) {
				grapple_state = HOOKING_PLAYER_TO_DYNAMIC_OBJECT;
			}
			else if (go->entity_type == EntityType::ENEMY) {
				grapple_state = HOOKING_OBJECT_TO_PLAYER;
			}
		}
		else {
			grapple_state = HOOKING_PLAYER_TO_STATIC_OBJECT;
		}
	}
}

void GrappleFistWeapon::Attack() {
	grapple_state = SHOOTING;
	snapshotted_attack_direction = GetAttackDirection();
}

void GrappleFistWeapon::ResetWeapon() {
	WeaponEntity::ResetWeapon();
	grapple_state = GRAPPLE_STATE::INACTIVE;
	travel_duration = 0.0f;
	grappled_object = nullptr;
	snapshotted_attack_direction = { 0.0f, 0.0f };
}