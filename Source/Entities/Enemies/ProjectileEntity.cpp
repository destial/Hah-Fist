#include "ProjectileEntity.hpp"
#include "../StaticEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
ProjectileEntity::ProjectileEntity(AEVec2 pos, f32 speed) : EnemyEntity(pos, { 1.f,0.f }, speed)
{
	sprite = AssetManager::GetSpriteSheet(ASSET_PROJECTILE_ENEMY_SPRITE, 7, 8);
	animationFrame = 1.f / (8.f * 7.f);
}

ProjectileEntity::~ProjectileEntity()
{
	// Empty by design
}

void ProjectileEntity::PreUpdate(const f32& dt)
{
	EnemyEntity::PreUpdate(dt);
}

void ProjectileEntity::Update(const f32& dt)
{
	EnemyEntity::Update(dt);
}

void ProjectileEntity::PostUpdate(const f32& dt)
{
	GameObjectEntity::PostUpdate(dt);
}

void ProjectileEntity::Render()
{
	EnemyEntity::Render();
}

void ProjectileEntity::OnCollide(GameObjectEntity* go)
{
	EnemyEntity::OnCollide(go);
	if (go->go_type == PhysicsType::DYNAMIC) {
		if (EnemyEntity* e = dynamic_cast<EnemyEntity*>(go)) {
			e->FlipDir();
			return;
		}
		SwitchState(FSM::IDLE, 2.f);
	}
	else if (go->go_type == PhysicsType::STATIC) {
		if (StaticEntity* se = dynamic_cast<StaticEntity*>(go)) {
			if (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_WALL) {
				if (position.y - scale.y * 0.5f < go->position.y + go->scale.y * 0.5f) {
					SwitchState(FSM::IDLE, 2.f);
				}
			}
		}
	}
}

void ProjectileEntity::OnHit()
{
}

void ProjectileEntity::OnIdle(const f32& dt)
{
	if (currentRow > 1)
		currentRow = 0;
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (currentRow == 0) {
			if (++currentCol >= 7) {
				currentCol = 0;
				++currentRow;
			}
		}
		else if (currentRow == 1) {
			if (++currentCol >= 1) {
				currentCol = 0;
				++currentRow;
			}
		}
	}
	//velocity.x = 0.f;
	//if (stateTimer < 0.f) {
	//	FlipDir(); // Flip the direction it is travelling.
	//	SwitchState(FSM::PATROL);
	//	return;
	//}
}

void ProjectileEntity::OnPatrol(const f32& dt)
{
	EnemyEntity::OnPatrol(dt);
}

void ProjectileEntity::OnChase(const f32& dt)
{
	EnemyEntity::OnChase(dt);
}

void ProjectileEntity::OnStun(const f32& dt)
{
	EnemyEntity::OnStun(dt);
}

void ProjectileEntity::OnDead(const f32& dt)
{
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
