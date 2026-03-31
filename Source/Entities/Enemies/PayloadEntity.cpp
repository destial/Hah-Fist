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
	attack_range = BOSS2ATTACKRANGE;
	land_timer = 0.f;
	base_projectiles = BOSS2EXTRAPROJECTILES;
	extra_projectiles = BOSS2BASEPROJECTILES;
	inner_state = INNERFSM::JUMP;
	frictionMultiplier = BOSS2FRICTION;
}

PayloadEntity::~PayloadEntity() {
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

void PayloadEntity::OnIdle(const f32&) {
	if (boss_activated) {
		SwitchState(FSM::CHASE);
	}
}

void PayloadEntity::OnChase(const f32& dt) {
	switch (inner_state) {
	case INNERFSM::JUMP:
	{
		dir.x = (Utils::RandRange(0.f, 1.f) < 0.5f) ? -1.f : 1.f;
		velocity.x = dir.x * BOSS2JUMPVELX;
		float t = AEClamp(position.y / BOSS2ROOMMAXHEIGHT, 0.f, 1.f);
		velocity.y = BOSS2JUMPVELY * (1.f - t * t);
		inner_state = INNERFSM::LAND;
		land_timer = land_cooldown;
		break;
	}
	case INNERFSM::LAND:
	{
		land_timer -= dt;
		if (land_timer < 0.f)
		{
			AEVec2 Pos{ position.x,  position.y - scale.y * 0.7f };
			AEVec2 platformDir{ 0.f, -1.f };
			MovingPlatformEntity* platform = new MovingPlatformEntity(Pos, platformDir, false , 0.2f , 5.0f);
			platform->mesh = MeshRenderer::GetCenterRectMesh();
			platform->scale = { 7.f, 0.5f };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
			inner_state = INNERFSM::ATTACK;
		}
		break;
	}
	case INNERFSM::ATTACK:
	{
		int projectiles = base_projectiles + static_cast<int>(GetLowHealthFactor() * extra_projectiles);
		for (int i = 0; i < projectiles; i++)
		{
			AEVec2 Pos{ Utils::RandRange(position.x - attack_range,position.x + attack_range),  position.y };
			AEVec2 shootDir{ 0.f, -1.f };
			ShootProjectile(health / max_health, Pos, shootDir);
		}
		inner_state = INNERFSM::JUMP;
		float stunTime = 1.f + (1.f * GetLowHealthFactor());
		SwitchState(FSM::STUN, stunTime);
		break;
	}
	default:
		break;
	}
}

void PayloadEntity::OnStun(const f32&) {
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}

void PayloadEntity::OnDead() {
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
	{
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);

}