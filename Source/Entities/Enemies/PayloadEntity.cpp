#include "PayloadEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../StaticEntities/MovingPlatformEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"

PayloadEntity::PayloadEntity(AEVec2 pos) : ground{nullptr}, BossEntity(pos) {
	InitializeAnimatedSpriteData(ASSET_PAYLOAD_SPRITE, ASSET_PAYLOAD_SPRITE_ROWS, ASSET_PAYLOAD_SPRITE_COLUMNS, ASSET_PAYLOAD_SPRITE_SCALE);
	// Empty for now
	attackRange = BOSS2ATTACKRANGE;
	landTimer = 0.f;
	baseProjectiles = BOSS2EXTRAPROJECTILES;
	extraProjectiles = BOSS2BASEPROJECTILES;
	innerState = INNERFSM::JUMP;
	frictionMultiplier = BOSS2FRICTION;
}

PayloadEntity::~PayloadEntity() {
	// Empty by design
}

void PayloadEntity::PreUpdate(const f32& dt) {
	BossEntity::PreUpdate(dt);
}

void PayloadEntity::Update(const f32& dt) {
	BossEntity::Update(dt);
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
	BossEntity::PostUpdate(dt);
}

void PayloadEntity::Render() {
	BossEntity::Render();
}

void PayloadEntity::OnCollide(GameObjectEntity* go) {
	BossEntity::OnCollide(go);
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
		velocity.x = dir.x * BOSS2JUMPVELX;
		float t = AEClamp(position.y / BOSS2ROOMMAXHEIGHT, 0.f, 1.f);
		velocity.y = BOSS2JUMPVELY * (1.f - t * t);
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
		int projectiles = baseProjectiles + static_cast<int>(GetLowHealthFactor() * extraProjectiles);
		for (int i = 0; i < projectiles; i++)
		{
			AEVec2 Pos{ Utils::RandRange(position.x - attackRange,position.x + attackRange),  position.y };
			AEVec2 shootDir{ 0.f, -1.f };
			ShootProjectile(health / max_health, Pos, shootDir);
		}
		innerState = INNERFSM::JUMP;
		float stunTime = 1.f + (1.f * GetLowHealthFactor());
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