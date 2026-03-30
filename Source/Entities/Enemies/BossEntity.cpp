#include "BossEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"

BossEntity::BossEntity(AEVec2 pos) : ground{ nullptr }, EnemyEntity(pos, { 1.f,0.f }, 10.f, true) {
	health = DEFAULTBOSSMAXHEALTH;
	max_health = DEFAULTBOSSMAXHEALTH;
	bossActivated = DEFAULTBOSSACTIVATED;
	damage = DEFAULTBOSSDAMAGE;
	bossRoomCenter = position;
}

void BossEntity::PreUpdate(const f32& dt)
{
	EnemyEntity::PreUpdate(dt);
}

void BossEntity::Update(const f32& dt)
{
	EnemyEntity::Update(dt);
}

void BossEntity::PostUpdate(const f32& dt)
{
	EnemyEntity::PostUpdate(dt);
}

void BossEntity::Render()
{
	EnemyEntity::Render();
}

void BossEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
}

void BossEntity::OnHit()
{
	EnemyEntity::OnHit();
}

void BossEntity::OnIdle(const f32& dt)
{
}

void BossEntity::OnPatrol(const f32& dt)
{
}

void BossEntity::OnChase(const f32& dt)
{
}

void BossEntity::OnAttack(const f32& dt)
{
}

void BossEntity::OnStun(const f32& dt)
{
}

void BossEntity::OnDead(const f32& dt)
{
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}

BossEntity::~BossEntity() {
}
bool BossEntity::GetBossActivated()
{
	return bossActivated;
}

void BossEntity::SetBossActivation(bool activated)
{
	bossActivated = activated;
}

AEVec2 BossEntity::GetBossRoomCenter()
{
	return bossRoomCenter;
}

void BossEntity::ShootProjectile(float healthRatio, AEVec2 Pos, AEVec2 shootDir)
{
	AEVec2Normalize(&shootDir, &shootDir);
	f32 bulletSpeed = Utils::RandRange(BULLETMINSPEED, BULLETMAXSPEED);
	if (healthRatio > 0.5f)
	{
		MissileProjectile* bullet = new MissileProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
		bullet->scale = { BULLETSCALEX ,BULLETSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
	}
	else
	{
		ExplosiveProjectile* bullet = new ExplosiveProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
		bullet->scale = { BULLETSCALEX ,BULLETSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
	}
}
float BossEntity::GetLowHealthFactor()
{
	float healthRatio = health / max_health;
	float temp = (1.f - healthRatio) / 0.75f;
	temp = AEClamp(temp, 0.f, 1.f);
	return temp;
}