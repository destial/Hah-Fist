#include "IronsideEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Utils/Constant.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../StaticEntities/MovingPlatformEntity.hpp"
#include "../TriggerEntities/LaserEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
#include "../PlayerEntity.hpp"

IronsideEntity::IronsideEntity(AEVec2 pos) : BossEntity(pos) {
	InitializeAnimatedSpriteData(ASSET_IRONSIDE_SPRITE, ASSET_IRONSIDE_SPRITE_ROWS, ASSET_IRONSIDE_SPRITE_COLUMNS, ASSET_IRONSIDE_SPRITE_SCALE);
	pBody->gravityScale = 0;
	inner_state = INNERFSM::MOVE;
	go_type = PhysicsType::TRIGGER;

	//Initialising Values (Will be reinitialised during runtime)
	lane_to_go_to = GetRandomSpawnLane(curr_lane);
	curr_lane = lane_to_go_to;
	next_lane_to_spawn = curr_lane;
	dir_to_go = 1.f;
	target_y = LaneY[lane_to_go_to];
}

IronsideEntity::~IronsideEntity() {
}

void IronsideEntity::PreUpdate(const f32& dt) {
	BossEntity::PreUpdate(dt);
}

void IronsideEntity::Update(const f32& dt) {
	BossEntity::Update(dt);
}

void IronsideEntity::PostUpdate(const f32& dt) {
	//Using its own animation spritesheet not the default
	GameObjectEntity::PostUpdate(dt);
}

void IronsideEntity::Render() {
	BossEntity::Render();
}

void IronsideEntity::OnCollide(GameObjectEntity* go) {
	BossEntity::OnCollide(go);
}

void IronsideEntity::OnIdle(const f32& dt) {
	if (boss_activated) {
		velocity.x = BOSS3VELX;
		if (position.x >= boss_room_center.x + BOSS3OFFSETX)
		{
			velocity.x = 0;
			SwitchState(FSM::CHASE);
		}
	}
}

void IronsideEntity::OnPatrol(const f32& dt) {
}

void IronsideEntity::OnChase(const f32& dt) {
	switch (inner_state) {
	case INNERFSM::MOVE:
	{
		lane_to_go_to = GetRandomSpawnLane(curr_lane);
		target_y = LaneY[lane_to_go_to];
		float diff = target_y - position.y;
		dir_to_go = (diff > 0.f) ? 1.f : -1.f;
		velocity.y = (GetLowHealthFactor()* BOSS3VELY + BOSS3VELY) * dir_to_go;
		inner_state = INNERFSM::MOVING;
		break;
	}
	case INNERFSM::MOVING:
	{
		if ((dir_to_go > 0 && position.y >= target_y) ||
			(dir_to_go < 0 && position.y <= target_y))
		{
			velocity.y = 0;
			inner_state = INNERFSM::SPAWNPLATFORM1;
			position.y = target_y;
			curr_lane = lane_to_go_to;
		}
		break;
	}
	case INNERFSM::SPAWNPLATFORM1:
	{
		LANE lanetospawn = GetRandomSpawnLane(curr_lane);
		next_lane_to_spawn = 3 - lanetospawn - curr_lane;
		AEVec2 Pos{ position.x, LaneY[lanetospawn] };
		AEVec2 platformDir{ -1.f, 0.f };
		MovingPlatformEntity* platform = new MovingPlatformEntity(Pos, platformDir, false, BOSS3PLATFORMSPEED, BOSS3PLATFORMLIFETIME);
		platform->mesh = MeshRenderer::GetCenterRectMesh();
		platform->scale = { BOSS3PLATFORMSCALEX, BOSS3PLATFORMSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
		inner_state = INNERFSM::SHOOTPROJECTILE;
		StunTimerBasedOnHealth();
		break;
	}
	case INNERFSM::SHOOTPROJECTILE:
	{
		Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>();
		if (!player) return;
		AEVec2 Pos{ position.x, LaneY[next_lane_to_spawn] };
		AEVec2 shootDir{ player->position.x - Pos.x,player->position.y - Pos.y };
		ShootProjectile(health / max_health, Pos, shootDir);
		Pos = AEVec2{ position.x, LaneY[3 - next_lane_to_spawn - curr_lane] };
		ShootProjectile(health / max_health, Pos, shootDir);

		inner_state = INNERFSM::SPAWNPLATFORM2;
		StunTimerBasedOnHealth();
		break;
	}
	case INNERFSM::SPAWNPLATFORM2:
	{
		AEVec2 Pos{ position.x, LaneY[next_lane_to_spawn] };
		AEVec2 platformDir{ -1.f, 0.f };
		MovingPlatformEntity* platform = new MovingPlatformEntity(Pos, platformDir, false, BOSS3PLATFORMSPEED, BOSS3PLATFORMLIFETIME);
		platform->mesh = MeshRenderer::GetCenterRectMesh();
		platform->scale = { BOSS3PLATFORMSCALEX, BOSS3PLATFORMSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(platform);
		inner_state = INNERFSM::LASER;
		StunTimerBasedOnHealth();
		break;
	}
	case INNERFSM::LASER:
	{
		currentRow = 1;
		if (currentCol == ASSET_IRONSIDE_SPRITE_COLUMNS - 1)
		{
			LaserEntity* laser = new LaserEntity(this->position, this, damage);
			SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(laser);
			inner_state = INNERFSM::MOVE;
			StunTimerBasedOnHealth();
			currentRow = 0;
		}

		break;
	}
	default:
		break;
	}
}

void IronsideEntity::OnStun(const f32& dt) {
	
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}

void IronsideEntity::OnDead(const f32& dt) {
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene()))
	{
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);

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
