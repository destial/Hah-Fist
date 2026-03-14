#include "EnemyEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../DropEntities/CoinEntity.hpp"
#include "../../Scenes/GameScene.hpp"
EnemyEntity::EnemyEntity(AEVec2 pos, AEVec2 dir, f32 speed) 
: GameObjectEntity{ pos }, state{ FSM::IDLE }, dir{ dir }, speed{ speed }, stateTimer{ 1.f }
{
	sprite = AssetManager::GetSpriteSheet(ASSET_BASEENEMY_SPRITE, 3, 3);
	mesh = nullptr;
	animationTimer = 0.f;
	animationFrame = 1.f / (3.f * 3.f);
	currentRow = currentCol = 0;
	scale = { 5.f * (static_cast<f32>(sprite->image->width) / sprite->image->height), 5.f };
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
	GameObjectEntity::PostUpdate(dt);
	currentRow = 0;
	if (velocity.x > 0) {
		currentRow = 1;
	}
	if (velocity.x < 0) {
		currentRow = 2;
	}
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 3) {
			currentCol = 0;
		}
	}
}

void EnemyEntity::Render()
{
	sprite->Render(transform, color, currentRow, currentCol);
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
	if (!Utils::RayHitAny({ position.x + std::abs(scale.x) * dir.x * 0.5f, position.y - std::abs(scale.x) * 0.5f }, AEVec2{ 0.f, -1.f }, GameScene::GetStaticEntities())) {
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
