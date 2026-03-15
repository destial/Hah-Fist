#include "TitanEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Utils/Constant.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/SpikeProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
#include "../PlayerEntity.hpp"

TitanEntity::TitanEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }, 10.f, true) {
	InitializeAnimatedSpriteData(ASSET_TITAN_SPRITE, ASSET_TITAN_SPRITE_ROWS, ASSET_TITAN_SPRITE_COLUMNS, ASSET_TITAN_SPRITE_SCALE);
	health = 500.f;
	max_health = 500.f;
	attackRange = 20.f;
	bossActivated = false;
	shootTimer = 0.f;
	jumpX = 15.f;
	jumpY = 50.f;
	bossRoomX = position.x;
	bossRoomY = 25.f;
	baseProjectiles = 3;
	extraProjectiles = 10;
	damage = 250.f;
	bossRoomCenter = position;
	
}

TitanEntity::~TitanEntity() {
	// Empty by design
}

void TitanEntity::PreUpdate(const f32& dt) {
	EnemyEntity::PreUpdate(dt);
}

void TitanEntity::Update(const f32& dt) {
	shootTimer -= dt;
	EnemyEntity::Update(dt);
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

	EnemyEntity::PostUpdate(dt);
}

void TitanEntity::Render() {
	EnemyEntity::Render();
}

void TitanEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
}

bool TitanEntity::GetBossActivated()
{
	return bossActivated;
}

void TitanEntity::SetBossActivation(bool activated)
{
	bossActivated = activated;
}

AEVec2 TitanEntity::GetBossRoomCenter()
{
	return bossRoomCenter;
}

void TitanEntity::OnIdle(const f32& dt) {
	// Trooper's idle behaviour
	
	if (bossActivated) {
		SwitchState(FSM::CHASE);
	}
}

void TitanEntity::OnPatrol(const f32& dt) {
	// Trooper's patrol behaviour
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}

void TitanEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
	Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>();
	if (!player) return;

	dir.x = (player->position.x > position.x) ? 1.f : -1.f;
	float healthRatio = health / max_health;
	float temp = (1.f - healthRatio) / 0.75f;
	temp = AEClamp(temp, 0.f, 1.f);
	velocity.x = dir.x * (jumpX + temp * jumpX);
	velocity.y = jumpY;
	
	SwitchState(FSM::STUN, 3.f);


}

void TitanEntity::OnStun(const f32& dt) {
	// Trooper's stun behaviour
	//velocity.x = 0.f;
	// Spawn falling projectiles periodically
	AEVec2 contactPt, normal;
	f32 timeCollide;
	if (pBody->is_standing_above && shootTimer < 0.f)
	{
		// Example spawn
		// SpawnProjectile({ player->position.x, ceilingHeight });
		//AEVec2 Pos{ Utils::RandRange(bossroommin.x,bossroommax.x), 100.f};
		CameraManager::GetInstance()->Shake(3.f, 5.f);
		float healthRatio = health / max_health;
		float temp = (1.f - healthRatio) / 0.75f;
		temp = AEClamp(temp, 0.f, 1.f);

		int projectiles = baseProjectiles + static_cast<int>(temp * extraProjectiles);
		for (int i = 0; i < projectiles; i++)
		{
			AEVec2 Pos{ Utils::RandRange(bossRoomX-attackRange,bossRoomX+attackRange),  bossRoomY };
			AEVec2 shootDir{ 0.f, -1.f };
			AEVec2Normalize(&shootDir, &shootDir);
			f32 bulletSpeed = Utils::RandRange(10, 20);

			MissileProjectile* bullet = new MissileProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
			bullet->scale = { BULLETSCALEX ,BULLETSCALEY };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
		}

		shootTimer = shootCooldown;
	}


	if (stateTimer < 0.f) {
		SwitchState(FSM::PATROL, 2.f);
	}
}

void TitanEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
	{
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
	


}
