#include "BossSpawnTriggerEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../UI/Debug.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../PlayerEntity.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Enemies/BossEntity.hpp"
#include "../../Managers/LevelManager.hpp"

BossSpawnTriggerEntity::BossSpawnTriggerEntity(AEVec2 pos) 
: StaticEntity{ STATIC_TYPE::TYPE_WALL, pos, 1.0f, CollisionShape::AABB, PhysicsType::TRIGGER } {
	color = Color{ 0.5f, 0.f, 0.f, 1.f };
	mesh = MeshRenderer::GetCenterRectMesh();
}

BossSpawnTriggerEntity::~BossSpawnTriggerEntity() {} // Empty dtor

void BossSpawnTriggerEntity::PreUpdate(const f32&) {
	if (!player_inside && player_entered) {
		PlayerEntity* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<PlayerEntity>();
		if (player->position.x > position.x) {
			if (player->position.x - player->GetHalfSize().x - 2.f > position.x + GetHalfSize().x) {
				go_type = PhysicsType::STATIC;
				color = Color{ 1.f, 0.5f, 0.f, 0.f };
				AEAudioPlay(AssetManager::GetAudio(ASSET_BOSS_SPAWN_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
				player_entered = false;

				BossEntity* boss = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<BossEntity>();
				boss->SetBossActivation(true);

			}
		}
		else {
			player_entered = false;
		}
	}
}

void BossSpawnTriggerEntity::PostUpdate(const f32&) {
	player_inside = false;
}

void BossSpawnTriggerEntity::OnCollide(GameObjectEntity* go) {
	if (go->entity_type == EntityType::PLAYER) {
		player_entered = true;
		player_inside = true;
	}
}