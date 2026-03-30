#include "TitanEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/SpikeProjectile.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
#include "../PlayerEntity.hpp"

TitanEntity::TitanEntity(AEVec2 pos) : ground{nullptr}, BossEntity(pos) {
	InitializeAnimatedSpriteData(ASSET_TITAN_SPRITE, ASSET_TITAN_SPRITE_ROWS, ASSET_TITAN_SPRITE_COLUMNS, ASSET_TITAN_SPRITE_SCALE);
	attack_range = BOSS1ATTACKRANGE;
	shoot_timer = 0.f;
	jump_x = BOSS1JUMPVELX;
	jump_y = BOSS1JUMPVELY;
	base_projectiles = BOSS1BASEPROJECTILES;
	extra_projectiles = BOSS1EXTRAPROJECTILES;
	
}

TitanEntity::~TitanEntity() {
}

void TitanEntity::PreUpdate(const f32& dt) {
	BossEntity::PreUpdate(dt);
}

void TitanEntity::Update(const f32& dt) {
	shoot_timer -= dt;
	BossEntity::Update(dt);
}

void TitanEntity::PostUpdate(const f32& dt) {
	currentRow = 1;
	if (velocity.x != 0) {
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
	}
	if (velocity.y != 0.f)
	{
		currentRow = 2;
	}

	BossEntity::PostUpdate(dt);
}

void TitanEntity::Render() {
	BossEntity::Render();
}

void TitanEntity::OnCollide(GameObjectEntity* go) {
	BossEntity::OnCollide(go);
}

void TitanEntity::OnIdle(const f32& dt) {	
	if (boss_activated) {
		SwitchState(FSM::CHASE);
	}
}

void TitanEntity::OnPatrol(const f32& dt) {
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}

void TitanEntity::OnChase(const f32& dt) {
	Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>();
	if (!player) return;

	dir.x = (player->position.x > position.x) ? 1.f : -1.f;
	float healthRatio = health / max_health;
	float temp = (1.f - healthRatio) / 0.75f;
	temp = AEClamp(temp, 0.f, 1.f);
	velocity.x = dir.x * (jump_x + temp * jump_x);
	velocity.y = jump_y;
	
	SwitchState(FSM::STUN, 3.f);


}

void TitanEntity::OnStun(const f32& dt) {
	if (pBody->is_standing_above && shoot_timer < 0.f)
	{
		CameraManager::GetInstance()->Shake(3.f, 5.f);
		float healthRatio = health / max_health;
		float temp = (1.f - healthRatio) / 0.75f;
		temp = AEClamp(temp, 0.f, 1.f);

		int projectiles = base_projectiles + static_cast<int>(temp * extra_projectiles);
		for (int i = 0; i < projectiles; i++)
		{
			AEVec2 Pos{ Utils::RandRange(boss_room_center.x-attack_range,boss_room_center.x +attack_range), BOSS1ROOMPOSY };
			AEVec2 shootDir{ 0.f, -1.f };
			AEVec2Normalize(&shootDir, &shootDir);
			f32 bulletSpeed = Utils::RandRange(BULLETMINSPEED, BULLETMAXSPEED);

			SpikeProjectile* spike = new SpikeProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
			spike->scale = { BULLETSCALEX ,BULLETSCALEY };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(spike);
		}

		shoot_timer = shoot_cooldown;
	}


	if (stateTimer < 0.f) {
		SwitchState(FSM::PATROL, 2.f);
	}
}

void TitanEntity::OnDead(const f32& dt) {
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
	{
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
