#include "BossSpawnTriggerEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../UI/Debug.hpp"
#include "../../Managers/SceneManager.hpp"
#include "../PlayerEntity.hpp"
#include "../../Managers/AssetManager.hpp"

BossSpawnTriggerEntity::BossSpawnTriggerEntity(AEVec2 pos) : StaticEntity(STATIC_TYPE::TYPE_WALL, pos, 1.0f, CollisionShape::AABB, PhysicsType::TRIGGER)
{
	this->color = Color{ 0.5f, 0.f, 0.f, 1.f };
	mesh = MeshRenderer::GetCenterRectMesh();
}

BossSpawnTriggerEntity::~BossSpawnTriggerEntity()
{

}

void BossSpawnTriggerEntity::PreUpdate(const f32& dt)
{
	if (!player_inside && player_entered)
	{
		Player* player = SceneManager::GetInstance()->GetCurrentScene()->GetFirstEntityOfType<Player>();
		if (player->position.x > position.x)
		{
			if (player->position.x - player->GetHalfSize().x - 2.f > position.x + GetHalfSize().x)
			{
				this->go_type = PhysicsType::STATIC;
				this->color = Color{ 1.f, 0.5f, 0.f, 0.f };
				AEAudioPlay(AssetManager::GetAudio(ASSET_BOSS_SPAWN_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
				player_entered = false;
			}
		}
		else
		{
			player_entered = false;
		}
	}
}

void BossSpawnTriggerEntity::PostUpdate(const f32& dt)
{
	player_inside = false;
}

void BossSpawnTriggerEntity::OnCollide(GameObjectEntity* go)
{
	if (go->entity_type == EntityType::PLAYER)
	{
		player_entered = true;
		player_inside = true;
	}
}