#include "ProjectileEntity.hpp"
#include "../PlayerEntity.hpp"
#include "../StaticEntity.hpp"
#include "../Projectiles/MissileProjectile.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/Constant.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../../Scenes/GameScene.hpp"
#include "../../UI/Debug.hpp" // For debugging remove later
#define PROJECTILE_ENTITY_LINE_OF_SIGHT 5.f
#define ATTACK_RANGE 2.0f

std::ostream& operator<<(std::ostream& os, const EnemyEntity::FSM& state)
{
	switch (state) {
	case EnemyEntity::FSM::IDLE: {
		return os << "IDLE";
	}
	case EnemyEntity::FSM::PATROL: {
		return os << "PATROL";
	}
	case EnemyEntity::FSM::CHASE: {
		return os << "CHASE";
	}
	case EnemyEntity::FSM::ATTACK: {
		return os << "ATTACK";
	}
	case EnemyEntity::FSM::STUN: {
		return os << "STUN";
	}
	case EnemyEntity::FSM::DEAD: {
		return os << "DEAD";
	}
	default:
		break;
	}
}


ProjectileEntity::ProjectileEntity(AEVec2 pos, f32 speed) : EnemyEntity(pos, { 1.f,0.f }, speed, true)
{
	InitializeAnimatedSpriteData(ASSET_PROJECTILE_ENEMY_SPRITE, ASSET_PROJECTILE_ENEMY_SPRITE_ROWS, ASSET_PROJECTILE_ENEMY_SPRITE_COLUMNS, ASSET_PROJECTILE_ENEMY_SPRITE_SCALE);
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
	if (velocity.x < 0) {
		if (scale.x > 0)
			scale.x *= -1.f;
	}
	else if (velocity.x > 0) {
		if (scale.x < 0)
			scale.x *= -1.f;
	}
	/*SwitchState(FSM::ATTACK);
	currentRow = 4;
	currentCol = 0;*/
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
	// ProjectileEntity's idle behaviour
	velocity.x = 0.f;
	if (stateTimer < 0.f) {
		FlipDir(); // Flip the direction it is travelling.
		currentRow = 1;
		currentCol = 2;
		SwitchState(FSM::PATROL);
		return;
	}
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
}

void ProjectileEntity::OnPatrol(const f32& dt)
{
	EnemyEntity::OnPatrol(dt);
	// Checks if Player is within the line of sight, if it is Chase.
	if (Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>()) {
		if (Utils::RayHit({ position.x + std::abs(scale.x) * 0.5f * dir.x,position.y }, { PROJECTILE_ENTITY_LINE_OF_SIGHT * dir.x,0.f }, player)) {
			SwitchState(FSM::CHASE, 5.f);
			speed *= 1.1f; // Increases speed by 10%
			return;
		}
	}
	if (currentRow > 3) {
		currentRow = 1;
		currentCol = 2;
	}
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 7) {
			currentCol = 0;
			++currentRow;
		}
	}
}

void ProjectileEntity::OnChase(const f32& dt)
{
	// Checks if there exists a player
	if (Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>()) {
		dir.x = player->position.x - position.x;
		AEVec2Normalize(&dir, &dir);
		if (abs(velocity.x) < speed)
		{
			velocity.x += dir.x * speed;
		}
		if (Utils::RayHit({ position.x + std::abs(scale.x) * 0.5f * dir.x,position.y }, { ATTACK_RANGE * dir.x, 0.f },player)) {
			SwitchState(FSM::ATTACK, 5.f);
			currentRow = 4;
			currentCol = 0;
		}
		if (stateTimer <= 0) {
			dir.x = 1.f;
			speed = INITIAL_SPEED;
			SwitchState(FSM::IDLE);
		}
		if (currentRow > 3) {
			currentRow = 1;
			currentCol = 2;
		}
		if ((animationTimer += dt) > animationFrame) {
			animationTimer = 0.f;
			if (++currentCol >= 7) {
				currentCol = 0;
				++currentRow;
			}
		}
	}
	else {
		SwitchState(FSM::IDLE);
	}
}

void ProjectileEntity::OnAttack(const f32& dt) {
	if (stateTimer <= 0.f) {
		SwitchState(FSM::PATROL);
	}
	// Animations
	if (currentRow > 6) {
		currentRow = 4;
		currentCol = 0;
	}
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (currentRow != 6) {
			if (++currentCol >= 8) {
				currentCol = 0;
				++currentRow;
			}
		}
		else {
			if (++currentCol >= 6) {
				++currentRow;
			}
		}
	}
	// Exact frame / Sprite where the Projectile Enemy launches projectile Row 4, Col 7
	if (currentRow == 4 && currentCol == 7) {
		f32 bulletSpeed = 30.f;
		MissileProjectile* bullet = new MissileProjectile(this->position, dir, bulletSpeed, damage, this);
		bullet->scale = { BULLETSCALEX, BULLETSCALEY };
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);
	}
}

void ProjectileEntity::OnStun(const f32& dt)
{
	EnemyEntity::OnStun(dt);
}

void ProjectileEntity::OnDead(const f32& dt)
{
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
}
