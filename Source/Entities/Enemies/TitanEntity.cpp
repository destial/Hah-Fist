#include "TitanEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../PlayerEntity.hpp"

TitanEntity::TitanEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }) {
	sprite = AssetManager::GetSpriteSheet(ASSET_TROOPER_SPRITE, 3, 3);
	this->health = 500;
	this->max_health = 500;
	shootTimer = 0.f;
}

TitanEntity::~TitanEntity() {
	// Empty by design
}

void TitanEntity::PreUpdate(const f32& dt) {
	EnemyEntity::PreUpdate(dt);
}

void TitanEntity::Update(const f32& dt) {
	//EnemyEntity::Update(dt);
	GameObjectEntity::Update(dt);
	stateTimer -= dt;
	shootTimer -= dt;
	switch (state) {
	case FSM::IDLE:
	{
		OnIdle(dt);
		break;
	}
	case FSM::CHASE:
	{
		OnChase(dt);
		break;
	}
	case FSM::STUN:
	{
		OnStun(dt);
		break;
	}
	case FSM::DEAD:
	{
		OnDead(dt);
		break;
	}
	default:
		break;
	}
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
	velocity.x = 0.f;
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}

void TitanEntity::OnPatrol(const f32& dt) {
	// Trooper's patrol behaviour
}

void TitanEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
	Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>();


	if (!player) return;

	dir.x = (player->position.x > position.x) ? 1.f : -1.f;

	velocity.x = dir.x * 15.f;
	velocity.y = 50.f;
	position.y += 0.5f;
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
		for (int i = 0; i < 3; i++)
		{
			AEVec2 Pos{ Utils::RandRange(-90,-60),  25 };
			AEVec2 shootDir{ 0.f, -1.f };
			AEVec2Normalize(&shootDir, &shootDir);
			f32 bulletSpeed = Utils::RandRange(5, 20);
			MissileProjectile* bullet = new MissileProjectile(Pos, shootDir, bulletSpeed, this->damage, this);
			bullet->scale = { 1.f, 0.5f };
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
		}
		//Put for loop to spawn a few

		shootTimer = shootCooldown;
	}


	if (stateTimer < 0.f) {
		SwitchState(FSM::IDLE, 2.f);
	}
}

void TitanEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
}
