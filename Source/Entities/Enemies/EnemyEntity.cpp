#include "EnemyEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../DropEntities/CoinEntity.hpp"
#include "../../Scenes/GameScene.hpp"
EnemyEntity::EnemyEntity(AEVec2 pos, AEVec2 dir, f32 speed, bool inherited) 
: GameObjectEntity{ pos }, state{ FSM::IDLE }, dir{ dir }, speed{ speed }, stateTimer{ 1.f }
{
	//Behaviour that would be called again by the inherited class, to prevent the extra load, this only runs in the base class
	if (!inherited)
	{
		InitializeAnimatedSpriteData(ASSET_BASEENEMY_SPRITE, ASSET_BASEENEMY_SPRITE_ROWS, ASSET_BASEENEMY_SPRITE_COLUMNS, ASSET_BASEENEMY_SPRITE_SCALE);
	}
	layer = RenderLayer::ENTITY;
	entity_type = EntityType::ENEMY;
}

EnemyEntity::~EnemyEntity()
{
}

void EnemyEntity::PreUpdate(const f32& dt)
{
	GameObjectEntity::PreUpdate(dt);
}

void EnemyEntity::Update(const f32& dt)
{
	GameObjectEntity::Update(dt);
	if (health <= 0.f && state != FSM::DEAD) {
		SwitchState(FSM::DEAD, 3.f);
	}
	stateTimer -= dt;
	switch (state) {
		case FSM::IDLE: 
		{
			OnIdle(dt);
			break;
		}
		case FSM::PATROL:
		{
			OnPatrol(dt);
			break;
		}
		case FSM::CHASE:
		{
			OnChase(dt);
			break;
		}
		case FSM::ATTACK:
		{
			OnAttack(dt);
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

void EnemyEntity::PostUpdate(const f32& dt)
{
	currentRow = 0;
	if (velocity.x > 0) {
		currentRow = 1;
	}
	if (velocity.x < 0) {
		currentRow = 2;
	}
	GameObjectEntity::PostUpdate(dt);
}

void EnemyEntity::Render()
{
	GameObjectEntity::Render();
}

void EnemyEntity::OnCollide(GameObjectEntity* go) {
	GameObjectEntity::OnCollide(go);
}

void EnemyEntity::OnHit()
{
	AEAudioPlay(AssetManager::GetAudio(ASSET_ENEMYHURT_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
}

void EnemyEntity::OnIdle(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnPatrol(const f32& dt)
{
	// Clamps the acceleration of object
	if (abs(velocity.x) < speed)
	{
		velocity.x += dir.x * speed;
	}
	// Checks if it is on the ledge.
	if (!Utils::RayHitAny({ position.x + std::abs(scale.x) * dir.x * 0.5f, position.y - scale.y * 0.5f }, AEVec2{ 0.f, -1.f }, GameScene::GetStaticEntities())) {
		velocity.x = 0.f;
		SwitchState(FSM::IDLE, 3.f);
	}
}

void EnemyEntity::OnChase(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnAttack(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnStun(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnDead(const f32& dt)
{
	CoinEntity* coin = new CoinEntity(position);
	SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(coin);
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}

void EnemyEntity::SwitchState(FSM newState, f32 timeInNewState)
{
	state = newState;
	stateTimer = timeInNewState;
}

EnemyEntity::FSM EnemyEntity::GetCurrentState()
{
	return state;
}

void EnemyEntity::FlipDir()
{
	dir.x *= -1.f;
}
