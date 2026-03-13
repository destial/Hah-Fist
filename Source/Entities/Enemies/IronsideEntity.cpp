#include "IronsideEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Utils/Constant.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../StaticEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
#include "../PlayerEntity.hpp"

IronsideEntity::IronsideEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }) {
	sprite = AssetManager::GetSpriteSheet(ASSET_TROOPER_SPRITE, 3, 3);
	// Empty for now
	health = 500.f;
	max_health = 500.f;
	attackRange = 20.f;
	bossActivated = true;
	shootTimer = 0.f;
	jumpX = 15.f;
	jumpY = 50.f;
	bossRoomX = position.x;
	bossRoomY = 25.f;
	baseProjectiles = 3;
	extraProjectiles = 10;
	pBody->gravityScale = 0;
	velocity.x = 5;
	currLane = LANE::LANE2;
	innerState = INNERFSM::MOVE;
	nextLanetospawn = 1;
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

		if (position.x >= bossRoomX + 10.f)
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
		velocity.y = 5 * dirtogo;
		innerState = INNERFSM::MOVING;
		break;
	}
	case INNERFSM::MOVING:
	{

		if ((dirtogo > 0 && position.y >= targetY - 0.1f) ||
			(dirtogo < 0 && position.y <= targetY + 0.1f))
		{
			std::cout << "test";
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
		nextLanetospawn = 6 - lanetospawn - currLane;
		AEVec2 Pos{ position.x, LaneY[lanetospawn] };
		StaticEntity* platform = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_PLATFORM, Pos);
		platform->mesh = MeshRenderer::GetCenterRectMesh();
		platform->scale = { 5.f, 0.5f };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
		innerState = INNERFSM::SHOOTPROJECTILE;
		break;
	}
	case INNERFSM::SHOOTPROJECTILE:
	{
		float healthRatio = health / max_health;
		float temp = (1.f - healthRatio) / 0.75f;
		temp = AEClamp(temp, 0.f, 1.f);

		int projectiles = baseProjectiles + static_cast<int>(temp * extraProjectiles);
		Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>();
		if (!player) return;
		AEVec2 Pos{ position.x,  position.y };
		AEVec2 shootDir{ player->position.x - position.x,player->position.y - position.y };
		AEVec2Normalize(&shootDir, &shootDir);
		for (int i = 0; i < projectiles; i++)
		{
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
		innerState = INNERFSM::SPAWNPLATFORM2;


		break;
	}
	case INNERFSM::SPAWNPLATFORM2:
	{
		AEVec2 Pos{ position.x, LaneY[nextLanetospawn] };
		StaticEntity* platform = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_PLATFORM, Pos);
		platform->mesh = MeshRenderer::GetCenterRectMesh();
		platform->scale = { 5.f, 0.5f };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
		innerState = INNERFSM::LASER;
		float healthRatio = health / max_health;
		float temp = (1.f - healthRatio) / 0.75f;
		temp = AEClamp(temp, 0.f, 1.f);
		float stunTime = 2.f * (1.f - temp);
		SwitchState(FSM::STUN, stunTime);
		break;
	}
	case INNERFSM::LASER:
	{
		innerState = INNERFSM::MOVE;
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