/*!
* @file BossEntity.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 28 February 2026
* @course CSD1451
* @brief Implementation of the BossEntity base class. This class provides
*        core functionality for all boss types, including health management,
*        activation state, projectile attacks, and common state behaviors
*        (idle, patrol, chase, attack, and stun). It also serves as a
*        foundation for derived boss classes to extend unique mechanics.
*/

#include "BossEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"

/*!
* @brief Constructs a BossEntity with an initial position
* @param pos - The starting position of the boss
*/
BossEntity::BossEntity(AEVec2 pos) : ground{ nullptr }, EnemyEntity(pos, { 1.f,0.f }, 10.f, true) {
	health = DEFAULTBOSSMAXHEALTH;
	max_health = DEFAULTBOSSMAXHEALTH;
	boss_activated = DEFAULTBOSSACTIVATED;
	damage = DEFAULTBOSSDAMAGE;
	boss_room_center = position;
}
/*!
* @brief Virtual destructor for BossEntity to allow proper cleanup of derived classes
*/
BossEntity::~BossEntity() {} // Empty dtor

/*!
* @brief Handles boss death by removing it from the current scene
*/
void BossEntity::OnDead() {
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
/*!
* @brief Gets whether the boss has been activated
* @return True if the boss is activated, false otherwise
*/
bool BossEntity::GetBossActivated() const {
	return boss_activated;
}
/*!
* @brief Sets the boss activation state
* @param activated - Boolean to activate or deactivate the boss
*/
void BossEntity::SetBossActivation(bool activated) {
	boss_activated = activated;
}
/*!
* @brief Retrieves the center position of the boss room
* @return AEVec2 representing the boss room center
*/
AEVec2 BossEntity::GetBossRoomCenter() const {
	return boss_room_center;
}
/*!
* @brief Idle state behavior of the boss
* @param dt - Delta time for frame update
*/
void BossEntity::OnIdle(const f32&) {
	// Empty body
}
/*!
* @brief Patrol state behavior of the boss
* @param dt - Delta time for frame update
*/
void BossEntity::OnPatrol(const f32&) {
	// Empty body
}

/*!
* @brief Chase state behavior of the boss
* @param dt - Delta time for frame update
*/
void BossEntity::OnChase(const f32&) {
	// Empty body
}
/*!
* @brief Attack state behavior of the boss
* @param dt - Delta time for frame update
*/
void BossEntity::OnAttack(const f32&) {
	// Empty body
}
/*!
* @brief Stun state behavior of the boss
* @param dt - Delta time for frame update
*/
void BossEntity::OnStun(const f32&) {
	// Empty body
}
/*!
* @brief Spawns a projectile based on the boss's current health
* @param healthRatio - Current health ratio of the boss (0.0 to 1.0)
* @param Pos - Spawn position of the projectile
* @param shootDir - Direction in which the projectile is fired
*/
void BossEntity::ShootProjectile(float healthRatio, AEVec2 Pos, AEVec2 shootDir) {
	//Normalizing shoot direction
	AEVec2Normalize(&shootDir, &shootDir);
	//Random Projectile Speed
	f32 bulletSpeed = Utils::RandRange(BULLETMINSPEED, BULLETMAXSPEED);
	//If Boss Health is below 50% spawn explosive projectile instead
	if (healthRatio > 0.5f) {
		//Normal Missile Spawn Code
		MissileProjectile* bullet = new MissileProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
		bullet->scale = { BULLETSCALEX ,BULLETSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
	}
	else {
		//Normal ExplosiveProjectile Spawn Code
		ExplosiveProjectile* bullet = new ExplosiveProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
		bullet->scale = { BULLETSCALEX ,BULLETSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
	}
}
/*!
* @brief Calculates a scaling factor based on the boss's remaining health
* @return A value between 0 and 1 indicating low-health intensity
*/
float BossEntity::GetLowHealthFactor() {
	//Gives a modifier to certain boss mechanics that maxes out when the boss is 75% health.
	float healthRatio = health / max_health;
	float temp = (1.f - healthRatio) / 0.75f;
	temp = AEClamp(temp, 0.f, 1.f);
	return temp;
}