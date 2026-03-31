/*!
* @file FingerGunEntity.cpp
* @author Mohammad Hafiz Bin Mohammad Kamarurrashid (mohammadhafiz.b@digipen.edu)
* @date 20th January 2026
* @course CSD1451
* @brief This source file declares the weapon class and the member functions.
*		 The weapon class object handles basic functionality and variables required
*		 by all inherited weapons.
*/
#include "FingerGunEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/Constant.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../../Managers/SceneManager.hpp"

FingerGunWeapon::FingerGunWeapon(AEVec2 pos) : WeaponEntity{pos} {
	weaponChannels = false;
	cd_duration = 0.5f;
	image = AssetManager::GetTexture(ASSET_FINGERGUN_IMAGE);
}

FingerGunWeapon::~FingerGunWeapon() {} // Empty dtor

void FingerGunWeapon::Update(const f32& dt) {
	if (!isActive)
		return;
	
	WeaponEntity::Update(dt);
}

void FingerGunWeapon::OnCollide(GameObjectEntity*) {
	// Empty body
}

void FingerGunWeapon::Attack() {
	f32 bulletSpeed = BULLETPLAYERSPEED;
	ExplosiveProjectile* bullet = new ExplosiveProjectile{ position, GetAttackDirection(), bulletSpeed, damage, player_entity };
	bullet->scale = { 1.f, 0.5f };
	SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
}