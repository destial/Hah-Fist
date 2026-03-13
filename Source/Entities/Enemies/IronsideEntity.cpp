#include "IronsideEntity.hpp"
#include "../../Utils/Utils.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/ExplosiveProjectile.hpp"
#include "../StaticEntity.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../Scenes/BaseScene.hpp"  
#include "../../Managers/SceneManager.hpp"

IronsideEntity::IronsideEntity(AEVec2 pos) : ground{nullptr}, EnemyEntity(pos, { 1.f,0.f }) {
	sprite = AssetManager::GetSpriteSheet(ASSET_TROOPER_SPRITE, 3, 3);
	// Empty for now
	health = 500.f;
	max_health = 500.f;
	attackRange = 20.f;
	bossActivated = false;
	shootTimer = 0.f;
	jumpX = 15.f;
	jumpY = 50.f;
	bossRoomX = position.x;
	bossRoomY = 25.f;
	baseProjectiles = 3;
	extraProjectiles = 10;
	pBody->gravityScale = 0;
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
		velocity.x += 5 * dt;
		if (position.x >= bossRoomX + 10.f)
		{

			SwitchState(FSM::CHASE);
		}
	}
}

void IronsideEntity::OnPatrol(const f32& dt) {
	// Trooper's patrol behaviour
}

void IronsideEntity::OnChase(const f32& dt) {
	// Trooper's chase behaviour
}

void IronsideEntity::OnStun(const f32& dt) {
	// Trooper's stun behaviour
}

void IronsideEntity::OnDead(const f32& dt) {
	// Trooper's death behaviour
	SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);

}
