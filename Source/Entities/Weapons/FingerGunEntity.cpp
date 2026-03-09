#include "FingerGunEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../../Managers/SceneManager.hpp"

FingerGunWeapon::FingerGunWeapon(AEVec2 pos, GameObjectEntity* player) : Weapon(pos, player)
{
	weaponChannels = false;
	cd_duration = 0.25f;
	image = AssetManager::GetTexture("Assets/fist3.png");
}
FingerGunWeapon::~FingerGunWeapon()
{

}

void FingerGunWeapon::PreUpdate(const f32& dt)
{
	Weapon::PreUpdate(dt);
}

void FingerGunWeapon::Update(const f32& dt)
{
	if (!isActive)
	{
		return;
	}
	Weapon::Update(dt);
	
}

void FingerGunWeapon::Render()
{
	Weapon::Render();
}


void FingerGunWeapon::Attack()
{
	f32 bulletSpeed = 5.f;
	f32 bulletDamage = 25.f;
	MissileProjectile* bullet = new MissileProjectile(this->position, GetAttackDirection(), bulletSpeed, bulletDamage, this->player_entity);
	SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
}

void FingerGunWeapon::ResetWeapon()
{
	Weapon::ResetWeapon();
}