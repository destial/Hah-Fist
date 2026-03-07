#include "SpiderEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../StaticEntity.hpp"
SpiderEntity::SpiderEntity(AEVec2 pos, f32 speed, bool spawnHatchlings) : ground{ nullptr }, bSpawnHatchlings{spawnHatchlings}, EnemyEntity(pos, {1.f,0.f}, speed)
{
	sprite = AssetManager::GetSpriteSheet("Assets/spider_enemy.png", 4, 6);
	animationFrame = 1.f / (4.f * 6.f);
}

SpiderEntity::~SpiderEntity()
{
	// Empty by design
}

void SpiderEntity::PreUpdate(const f32& dt)
{
	EnemyEntity::PreUpdate(dt);
}

void SpiderEntity::Update(const f32& dt)
{
	EnemyEntity::Update(dt);
}

void SpiderEntity::PostUpdate(const f32& dt)
{
	GameObjectEntity::PostUpdate(dt);
	// Animation
	currentRow = 0;
	if (velocity.x > 0) {
		currentRow = 1;
	}
	if (velocity.x < 0) {
		currentRow = 2;
	}/*
	if (death) {
		currentRow = 3
	}*/
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 6) {
			currentCol = 0;
		}
	}
}

void SpiderEntity::Render()
{
	EnemyEntity::Render();
}

void SpiderEntity::OnCollide(GameObjectEntity* go)
{
	EnemyEntity::OnCollide(go);
	StaticEntity* se = dynamic_cast<StaticEntity*>(go);
	if(se != nullptr)
		ground = (dynamic_cast<StaticEntity*>(go)->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_PLATFORM) ? go : nullptr; //go->go_type == PhysicsType::STATIC ? go : nullptr;
	if (go->go_type == PhysicsType::DYNAMIC || dynamic_cast<StaticEntity*>(go)->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_WALL) {
		SwitchState(FSM::IDLE, 2.f);
	}
}

void SpiderEntity::OnIdle(const f32& dt)
{
	// Spider's idle behaviour
	velocity.x = 0.f;
	if (stateTimer < 0.f) {
		dir.x *= -1.f; // Flip the direction it is travelling.
		SwitchState(FSM::PATROL);
		return;
	}
}

void SpiderEntity::OnPatrol(const f32& dt)
{
	AEVec2 contactPt, normal;
	f32 timeCollide;
	// Clamps the acceleration of object
	if (abs(velocity.x) < speed)
	{
		velocity.x += dir.x * speed;
	}
	// Checks if it is on the ledge.
	if (ground != nullptr && !(Utils::RayAABB({ position.x + scale.x * dir.x * 0.5f, position.y }, AEVec2{ 0.f, -1.f }, ground, contactPt, normal, timeCollide))) {
		velocity.x = 0.f;
		SwitchState(FSM::IDLE, 2.f); // Switching of states
	}
}

void SpiderEntity::OnChase(const f32& dt)
{
}

void SpiderEntity::OnStun(const f32& dt)
{
}

void SpiderEntity::OnDead(const f32& dt)
{
	// Checks if this spider can spawn hatchlings
	if (bSpawnHatchlings) {
		SpiderEntity* baby = new SpiderEntity(position, 15.f, false);
		f32 dir = rand() % 2 ? -1.f : 1;
		baby->velocity = { AERandFloat() * 5.f, AERandFloat() * 5.f };
		baby->state = FSM::PATROL;
		baby->scale = { scale.x * 0.5f * dir, scale.y * 0.5f };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(baby);
	}
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
