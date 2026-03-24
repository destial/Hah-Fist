#include "IronsideEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Utils/Constant.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../StaticEntities/MovingPlatformEntity.hpp"
#include "../TriggerEntities/LaserEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
#include "../PlayerEntity.hpp"

IronsideEntity::IronsideEntity(AEVec2 pos) : EnemyEntity(pos, { 1.f,0.f }, 10.f, true) {
	InitializeAnimatedSpriteData(ASSET_TROOPER_SPRITE, ASSET_TROOPER_SPRITE_ROWS, ASSET_TROOPER_SPRITE_COLUMNS, ASSET_TROOPER_SPRITE_SCALE);
	// Empty for now
	health = DEFAULTBOSSMAXHEALTH;
	max_health = DEFAULTBOSSMAXHEALTH;
	damage = DEFAULTBOSSDAMAGE;
	bossActivated = DEFAULTBOSSACTIVATED;

	pBody->gravityScale = 0;

	innerState = INNERFSM::MOVE;
	go_type = PhysicsType::TRIGGER;
	//Initialising values so no errors
	currLane = LANE::LANE2;
	lanetogoto = LANE::LANE1;
	nextLanetospawn = 1;
	dirtogo = 1;
	targetY = 6.0f;
	bossRoomCenter = position;

}

IronsideEntity::~IronsideEntity() {
	// Empty by design
}

void IronsideEntity::PreUpdate(const f32& dt) {
	EnemyEntity::PreUpdate(dt);
}

void IronsideEntity::Update(const f32& dt) {
	EnemyEntity::Update(dt);
}

void IronsideEntity::PostUpdate(const f32& dt) {
	EnemyEntity::PostUpdate(dt);
}

void IronsideEntity::Render() {
	EnemyEntity::Render();
}

void IronsideEntity::OnCollide(GameObjectEntity* go) {
	EnemyEntity::OnCollide(go);
}

void IronsideEntity::OnIdle(const f32& dt) {
	// Trooper's idle behaviour
	if (bossActivated) {
		velocity.x = BOSS3VELX;
		if (position.x >= bossRoomCenter.x + BOSS3OFFSETX)
		{
			velocity.x = 0;
			SwitchState(FSM::CHASE);
		}
	}
}

void IronsideEntity::OnPatrol(const f32& dt) {
	// Trooper's patrol behaviour
}

void IronsideEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
	switch (innerState) {
	case INNERFSM::MOVE:
	{
		lanetogoto = GetRandomSpawnLane(currLane);
		targetY = LaneY[lanetogoto];
		float diff = targetY - position.y;
		dirtogo = (diff > 0.f) ? 1.f : -1.f;
		velocity.y = (GetLowHealthFactor()* BOSS3VELY + BOSS3VELY) * dirtogo;
		innerState = INNERFSM::MOVING;
		break;
	}
	case INNERFSM::MOVING:
	{
		if ((dirtogo > 0 && position.y >= targetY) ||
			(dirtogo < 0 && position.y <= targetY))
		{
			velocity.y = 0;
			innerState = INNERFSM::SPAWNPLATFORM1;
			position.y = targetY;
			currLane = lanetogoto;
		}
		break;
	}
	case INNERFSM::SPAWNPLATFORM1:
	{
		LANE lanetospawn = GetRandomSpawnLane(currLane);
		nextLanetospawn = 3 - lanetospawn - currLane;
		AEVec2 Pos{ position.x, LaneY[lanetospawn] };
		AEVec2 platformDir{ -1.f, 0.f };
		MovingPlatformEntity* platform = new MovingPlatformEntity(Pos, platformDir, false, BOSS3PLATFORMSPEED, BOSS3PLATFORMLIFETIME);
		platform->mesh = MeshRenderer::GetCenterRectMesh();
		platform->scale = { BOSS3PLATFORMSCALEX, BOSS3PLATFORMSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
		innerState = INNERFSM::SHOOTPROJECTILE;
		StunTimerBasedOnHealth();
		break;
	}
	case INNERFSM::SHOOTPROJECTILE:
	{


		ShootProjectile(health / max_health, LaneY[nextLanetospawn]);
		ShootProjectile(health / max_health, LaneY[3 - nextLanetospawn - currLane]);

		innerState = INNERFSM::SPAWNPLATFORM2;
		StunTimerBasedOnHealth();
		break;
	}
	case INNERFSM::SPAWNPLATFORM2:
	{
		AEVec2 Pos{ position.x, LaneY[nextLanetospawn] };
		AEVec2 platformDir{ -1.f, 0.f };
		MovingPlatformEntity* platform = new MovingPlatformEntity(Pos, platformDir, false, BOSS3PLATFORMSPEED, BOSS3PLATFORMLIFETIME);
		platform->mesh = MeshRenderer::GetCenterRectMesh();
		platform->scale = { BOSS3PLATFORMSCALEX, BOSS3PLATFORMSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
		innerState = INNERFSM::LASER;
		StunTimerBasedOnHealth();
		break;
	}
	case INNERFSM::LASER:
	{
		LaserEntity* laser = new LaserEntity(this->position, this, damage);
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(laser);
		innerState = INNERFSM::MOVE;
		StunTimerBasedOnHealth();
		break;
	}
	default:
		break;
	}
}

void IronsideEntity::OnStun(const f32& dt) {
	// Trooper's stun behaviour
	
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}

void IronsideEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
	{
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);

}

bool IronsideEntity::GetBossActivated()
{
	return bossActivated;
}

void IronsideEntity::SetBossActivation(bool activated)
{
	bossActivated = activated;
}

AEVec2 IronsideEntity::GetBossRoomCenter()
{
	return bossRoomCenter;
}
void IronsideEntity::ShootProjectile(float healthRatio,f32 posY)
{
	Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>();
	if (!player) return;
	AEVec2 Pos{ position.x, posY };
	AEVec2 shootDir{ player->position.x - Pos.x,player->position.y - Pos.y };
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
float IronsideEntity::GetLowHealthFactor()
{
	float healthRatio = health / max_health;
	float temp = (1.f - healthRatio) / 0.75f;
	temp = AEClamp(temp, 0.f, 1.f);
	return temp;
}
void IronsideEntity::StunTimerBasedOnHealth()
{
	float stunTime = 1.f + (1.f - GetLowHealthFactor());
	SwitchState(FSM::STUN, stunTime);
}
IronsideEntity::LANE IronsideEntity::GetRandomSpawnLane(IronsideEntity::LANE bossLane)
{
	int r = static_cast<int>(Utils::RandRange(0.f, 2.f));

	switch (bossLane)
	{
	case LANE::LANE1: return r ? LANE::LANE2 : LANE::LANE3;
	case LANE::LANE2: return r ? LANE::LANE1 : LANE::LANE3;
	case LANE::LANE3: return r ? LANE::LANE1 : LANE::LANE2;
	default: return LANE::LANE1;
	}
}
