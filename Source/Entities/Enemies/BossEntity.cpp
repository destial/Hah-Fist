#include "BossEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"

BossEntity::BossEntity(AEVec2 pos) : ground{ nullptr }, EnemyEntity(pos, { 1.f,0.f }, 10.f, true) {
	health = DEFAULTBOSSMAXHEALTH;
	max_health = DEFAULTBOSSMAXHEALTH;
	boss_activated = DEFAULTBOSSACTIVATED;
	damage = DEFAULTBOSSDAMAGE;
	boss_room_center = position;
}

BossEntity::~BossEntity() {} // Empty dtor

void BossEntity::OnDead() {
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}

bool BossEntity::GetBossActivated() const {
	return boss_activated;
}

void BossEntity::SetBossActivation(bool activated) {
	boss_activated = activated;
}

AEVec2 BossEntity::GetBossRoomCenter() const {
	return boss_room_center;
}

void BossEntity::OnIdle(const f32&) {
	// Empty body
}

void BossEntity::OnPatrol(const f32&) {
	// Empty body
}

void BossEntity::OnChase(const f32&) {
	// Empty body
}

void BossEntity::OnAttack(const f32&) {
	// Empty body
}

void BossEntity::OnStun(const f32&) {
	// Empty body
}

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

float BossEntity::GetLowHealthFactor() {
	//Gives a modifier to certain boss mechanics that maxes out when the boss is 75% health.
	float healthRatio = health / max_health;
	float temp = (1.f - healthRatio) / 0.75f;
	temp = AEClamp(temp, 0.f, 1.f);
	return temp;
}