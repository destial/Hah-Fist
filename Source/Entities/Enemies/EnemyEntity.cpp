#include "EnemyEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Utils/AEOverload.hpp"

EnemyEntity::EnemyEntity(AEVec2 pos, AEVec2 dir) : state{ FSM::IDLE }, dir{dir}, stateTimer{ 1.f }, GameObjectEntity(pos)
{
	sprite = AssetManager::GetSpriteSheet("Assets/test_enemy.png", 3, 3);
	mesh = nullptr;
	animationTimer = 0.f;
	animationFrame = 1.f / (3.f * 3.f);
	currentRow = currentCol = 0;
	scale = { 5.f * (static_cast<f32>(sprite->image->width) / sprite->image->height), 5.f };
	layer = 2;
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

void EnemyEntity::OnIdle(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnPatrol(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnChase(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnStun(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::OnDead(const f32& dt)
{
	// Empty for now
}

void EnemyEntity::SwitchState(FSM newState, f32 timeInNewState)
{
	state = newState;
	stateTimer = timeInNewState;
}
