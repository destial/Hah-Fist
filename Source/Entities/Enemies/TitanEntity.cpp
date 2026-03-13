#include "TitanEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/SpikeProjectile.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../PlayerEntity.hpp"

TitanEntity::TitanEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }) {
	sprite = AssetManager::GetSpriteSheet(ASSET_TROOPER_SPRITE, 3, 3);
	this->health = 500.f;
	this->max_health = 500.f;
	attackRange = 20.f;
	bossActivated = false;
	shootTimer = 0.f;
	jumpX = 15.f;
	jumpY = 50.f;
	bossRoomX = position.x;
	bossRoomY = 25.f;
	baseProjectiles = 3;
	extraProjectiles = 10;
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
	EnemyEntity::PostUpdate(dt);
}

void TitanEntity::Render() {
	EnemyEntity::Render();
}

void TitanEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
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
	if (pBody->state == PhysicsBody::STATE::ON_GROUND && shootTimer < 0.f)
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

			SpikeProjectile* bullet = new SpikeProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
			bullet->scale = { 1.f, 0.5f };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
		}
		//Put for loop to spawn a few

		shootTimer = shootCooldown;
	}


	if (stateTimer < 0.f) {
		SwitchState(FSM::PATROL, 2.f);
	}
}

void TitanEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
}
