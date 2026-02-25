#include "SerializationManager.hpp"
#include "AEEngine.h"
#include "../Entities/GameObjectEntity.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Utils/MeshRenderer.hpp"

namespace Serialization {
	enum EntityType {
		PLATFORM = 0, PLAYER, ENEMY, TROOPER
	};

	struct SerializedEntity {
		f32 x, y;
		f32 scale_x, scale_y, rotation;
		f32 health, damage, mass;
		EntityType type = PLATFORM;

		// TODO: add more serialization options if needed
	};

	SerializedEntity Serialize(BaseEntity* en) {
		SerializedEntity s{en->position.x, en->position.y, en->scale.x, en->scale.y, en->rotation};
		if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(en)) {
			s.health = go->health;
			s.damage = go->damage;
			s.type = EntityType::PLATFORM;
			s.mass = go->pBody->mass;

			if (Player* p = dynamic_cast<Player*>(en)) {
				s.type = EntityType::PLAYER;
			}
			else if (EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(en)) {
				if (TrooperEntity* trooper = dynamic_cast<TrooperEntity*>(enemy)) {
					s.type = EntityType::TROOPER;
				}
				else {
					s.type = EntityType::ENEMY;
				}
			}
		}
		return s;
	}

	BaseEntity* Unserialize(SerializedEntity const& en) {
		BaseEntity* entity = nullptr;
		switch (en.type) {
			case EntityType::PLATFORM: {
				entity = new GameObjectEntity(AEVec2{ en.x, en.y });
				if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity)) {
					go->go_type = GameObjectEntity::PhysicsType::STATIC;
					go->mesh = MeshRenderer::GetCenterRectMesh();
				}
				break;
			}
			case EntityType::PLAYER: {
				entity = new Player(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::TROOPER: {
				entity = new TrooperEntity(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::ENEMY: {
				entity = new TrooperEntity(AEVec2{ en.x, en.y });
				break;
			}

			default: break;
		}

		if (entity) {
			entity->scale = { en.scale_x, en.scale_y };
			entity->rotation = en.rotation;

			if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity)) {
				go->health = en.health;
				go->damage = en.damage;
				go->pBody->mass = en.mass;
			}
		}
		return entity;
	}

	void WriteToFile(const char* filename, std::vector<SerializedEntity> entities) {
	}

	std::vector<SerializedEntity> SerializeAll(std::vector<BaseEntity*>& scene_entities) {
		std::vector<SerializedEntity> vect;
		for (BaseEntity*& const en : scene_entities) {
			if (en->layer == BaseEntity::RenderLayer::UI)
				continue;

			vect.push_back(Serialize(en));
		}
		return vect;
	}

	std::vector<SerializedEntity> LoadFromFile(const char* filename) {
		std::vector<SerializedEntity> vect;
		return vect;
	}

	void LoadToMemory(std::vector<BaseEntity*>& scene_entities, std::vector<SerializedEntity>& serialized) {

	}
}