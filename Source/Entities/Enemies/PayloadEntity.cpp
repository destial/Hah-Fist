#include "PayloadEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Utils/Constant.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../StaticEntities/MovingPlatformEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"

PayloadEntity::PayloadEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }, 10.f, true) {
	InitializeAnimatedSpriteData(ASSET_PAYLOAD_SPRITE, ASSET_PAYLOAD_SPRITE_ROWS, ASSET_PAYLOAD_SPRITE_COLUMNS, ASSET_PAYLOAD_SPRITE_SCALE);
	// Empty for now
	health = 500.f;
	max_health = 500.f;
	attackRange = 10.f;
	bossActivated = false;
	landTimer = 0.f;
	jumpX = 15.f;
	jumpY = 35.f;
	baseProjectiles = 3;
	extraProjectiles = 2;
	innerState = INNERFSM::JUMP;
	frictionMultiplier = 100.f;
	damage = 25;
	bossRoomCenter = position;
}

PayloadEntity::~PayloadEntity() {
	// Empty by design
}

void PayloadEntity::PreUpdate(const f32& dt) {
	EnemyEntity::PreUpdate(dt);
}

void PayloadEntity::Update(const f32& dt) {
	EnemyEntity::Update(dt);
}

void PayloadEntity::PostUpdate(const f32& dt) {
	currentRow = 1;
	if (fabsf(velocity.x) > 0.1f) {
		currentRow = 0;
		if (velocity.x < 0)
		{
			if (this->scale.x > 0)
			{
				this->scale.x *= -1;
			}
		}
		else
		{
			if (this->scale.x < 0)
			{
				this->scale.x *= -1;
			}
		}
		if (AEVec2Length(&velocity) > 50.0f)
		{
			currentRow = 2;
		}
	}
	EnemyEntity::PostUpdate(dt);
}

void PayloadEntity::Render() {
	EnemyEntity::Render();
}

void PayloadEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
}

void PayloadEntity::OnIdle(const f32& dt) {
	// Trooper's idle behaviour
	if (bossActivated) {
		SwitchState(FSM::CHASE);
	}
}

void PayloadEntity::OnPatrol(const f32& dt) {
	// Trooper's patrol behaviour
}

void PayloadEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
	switch (innerState) {
	case INNERFSM::JUMP:
	{
		dir.x = (Utils::RandRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;
		velocity.x = dir.x * jumpX;
		velocity.y = jumpY;
		innerState = INNERFSM::LAND;
		landTimer = landCooldown;
		break;
	}
	case INNERFSM::LAND:
	{
		landTimer -= dt;
		if (landTimer < 0.f)
		{
			AEVec2 Pos{ position.x,  position.y - scale.y * 0.7f };
			AEVec2 platformDir{ 0.f, -1.f };
			MovingPlatformEntity* platform = new MovingPlatformEntity(Pos, platformDir, false , 0.2f , 5.0f);
			platform->mesh = MeshRenderer::GetCenterRectMesh();
			platform->scale = { 7.f, 0.5f };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
			innerState = INNERFSM::ATTACK;
		}
		break;
	}
	case INNERFSM::ATTACK:
	{
		float healthRatio = health / max_health;
		float temp = (1.f - healthRatio) / 0.75f;
		temp = AEClamp(temp, 0.f, 1.f);

		int projectiles = baseProjectiles + static_cast<int>(temp * extraProjectiles);
		for (int i = 0; i < projectiles; i++)
		{
			AEVec2 Pos{ Utils::RandRange(position.x - attackRange,position.x + attackRange),  position.y };
			AEVec2 shootDir{ 0.f, -1.f };
			AEVec2Normalize(&shootDir, &shootDir);
			f32 bulletSpeed = Utils::RandRange(10, 20);
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
		innerState = INNERFSM::JUMP;
		float stunTime = 2.f;
		SwitchState(FSM::STUN, stunTime);
		break;
	}
	default:
		break;
	}
}

void PayloadEntity::OnStun(const f32& dt) {
	// Trooper's stun behaviour
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}

}

void PayloadEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
	{
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);

}
bool PayloadEntity::GetBossActivated()
{
	return bossActivated;
}

void PayloadEntity::SetBossActivation(bool activated)
{
	bossActivated = activated;
}
AEVec2 PayloadEntity::GetBossRoomCenter()
{
	return bossRoomCenter;
}