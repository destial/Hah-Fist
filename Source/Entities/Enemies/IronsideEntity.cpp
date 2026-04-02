/*!
* @file IronsideEntity.cpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief Implementation of the IronsideEntity boss class. This class defines
*        behavior for a boss enemy that operates using a lane-based system,
*        spawning platforms, firing projectiles, and executing laser attacks.
*        The boss uses an internal state machine and scales behavior based on health.
*/
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
/*!
* @brief Constructs the IronsideEntity and initializes its movement,
*        lane system, and internal state machine
* @param pos - Initial position of the Ironside boss
* @return None
*/
IronsideEntity::IronsideEntity(AEVec2 pos) : BossEntity(pos) {
	//Initialising Sprite Data and Boss Variables
	InitializeAnimatedSpriteData(ASSET_IRONSIDE_SPRITE, ASSET_IRONSIDE_SPRITE_ROWS, ASSET_IRONSIDE_SPRITE_COLUMNS, ASSET_IRONSIDE_SPRITE_SCALE);
	//For gravityScale it should be set to 0 since i do not want it to follow the normal gravity that entities have
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
/*!
* @brief Destructor for IronsideEntity
* @return None
*/
IronsideEntity::~IronsideEntity() {
	// Empty body
}
/*!
* @brief Handles post-update logic using base GameObjectEntity animation handling
* @param dt - Delta time since last frame
* @return None
*/
void IronsideEntity::PostUpdate(const f32& dt) {
	//Using its own animation spritesheet not the default
	GameObjectEntity::PostUpdate(dt);
}
/*!
* @brief Handles idle state behavior and transitions to chase when activated
* @param dt - Delta time since last frame (unused)
* @return None
*/
void IronsideEntity::OnIdle(const f32&) {
	//Activate the Boss Fight
	if (boss_activated) {
		velocity.x = BOSS3VELX;
		if (position.x >= boss_room_center.x + BOSS3OFFSETX)
		{
			velocity.x = 0;
			SwitchState(FSM::CHASE);
		}
	}
}
/*!
* @brief Handles patrol state behavior (currently unused)
* @param dt - Delta time since last frame (unused)
* @return None
*/
void IronsideEntity::OnPatrol(const f32&) {
	// Empty body
}
/*!
* @brief Handles chase behavior using an internal FSM for movement, platform spawning,
*        projectile attacks, and laser execution
* @param dt - Delta time since last frame (unused)
* @return None
*/
void IronsideEntity::OnChase(const f32&) {
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
		PlayerEntity* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<PlayerEntity>();
		if (!player) return;
		AEVec2 Pos{ position.x, LaneY[next_lane_to_spawn] };
		AEVec2 shootDir{ player->position.x - Pos.x,player->position.y - Pos.y };
		ShootProjectile(health / max_health, Pos, shootDir);
		Pos = AEVec2{ position.x, LaneY[3 - next_lane_to_spawn - curr_lane] };
		shootDir = AEVec2{ player->position.x - Pos.x,player->position.y - Pos.y };
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
/*!
* @brief Handles stun state behavior and transitions back to chase when timer expires
* @param dt - Delta time since last frame (unused)
* @return None
*/
void IronsideEntity::OnStun(const f32&) {
	if (stateTimer < 0.f) {
		SwitchState(FSM::CHASE);
	}
}
/*!
* @brief Handles boss death by triggering win condition and removing the entity
* @return None
*/
void IronsideEntity::OnDead() {
	if (GameScene* game = dynamic_cast<GameScene*>(SceneManager::GetInstance()->GetCurrentScene())) {
		game->Win();
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);

}

/*!
* @brief Calculates stun duration based on current health and applies stun state
* @return None
*/
void IronsideEntity::StunTimerBasedOnHealth()
{
	float stunTime = 1.f + (1.f - GetLowHealthFactor());
	SwitchState(FSM::STUN, stunTime);
}
/*!
* @brief Selects a random lane different from the current boss lane
* @param bossLane - Current lane occupied by the boss
* @return A randomly selected lane that is not the current lane
*/
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
