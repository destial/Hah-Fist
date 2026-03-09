#include <fstream>
#include <iostream>
#include "SerializationManager.hpp"
#include "AEEngine.h"
#include "../Entities/GameObjectEntity.hpp"
#include "../Entities/StaticEntity.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/Enemies/SpiderEntity.hpp"
#include "../UI/BaseUI.hpp"
#include "../Utils/MeshRenderer.hpp"

namespace Serialization {

	SerializedEntity::SerializedEntity()
		: x{ 0 }, y{ 0 }, scale_x{ 0 }, scale_y{ 0 }, rotation{ 0 }, health{ 0 }, damage{ 0 }, mass{ 0 }, type{ EntityType::PLATFORM } {
	}

	SerializedEntity& SerializedEntity::operator=(SerializedEntity const& rhs) {
		x = rhs.x;
		y = rhs.y;
		scale_x = rhs.scale_x;
		scale_y = rhs.scale_y;
		rotation = rhs.rotation;
		type = rhs.type;
		damage = rhs.damage;
		health = rhs.health;
		mass = rhs.mass;

		return *this;
	}

	std::ostream& SerializedEntity::operator<< (std::ostream& rhs) const {
		return rhs << x << ',' << y << ',' << scale_x << ','
			<< scale_y << ',' << rotation << ',' << health
			<< ',' << damage << ',' << mass << ',' << type << '|';
	}

	std::istream& SerializedEntity::operator>> (std::istream& rhs) {
		char c;
		return rhs >> x >> c >> y >> c >> scale_x >> c >> scale_y
			>> c >> rotation >> c >> health >> c >> damage >> c
			>> mass >> c >> type >> c;
	}

	SerializedEntity Serialize(BaseEntity* en) {
		SerializedEntity s{};
		s.x = en->position.x;
		s.y = en->position.y;
		s.scale_x = en->scale.x;
		s.scale_y = en->scale.y;
		s.rotation = en->rotation;
		if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(en)) {
			s.health = go->health;
			s.damage = go->damage;
			s.type = EntityType::PLATFORM;
			s.mass = go->pBody->mass;
			if (StaticEntity* se = dynamic_cast<StaticEntity*>(en)) {
				s.type = (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_WALL) ? EntityType::WALL : EntityType::PLATFORM;
			}
			else if (Player* p = dynamic_cast<Player*>(en)) {
				s.type = EntityType::PLAYER;
			}
			else if (EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(en)) {
				if (TrooperEntity* trooper = dynamic_cast<TrooperEntity*>(enemy)) {
					s.type = EntityType::TROOPER;
				}
				else if (SpiderEntity* spider = dynamic_cast<SpiderEntity*>(enemy)) {
					s.type = EntityType::SPIDER;
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
				entity = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_PLATFORM,AEVec2{ en.x, en.y });
				if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity)) {
					//go->go_type = GameObjectEntity::PhysicsType::STATIC;
					go->mesh = MeshRenderer::GetCenterRectMesh();
				}
				break;
			}
			case EntityType::WALL: {
				entity = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_WALL,AEVec2{ en.x, en.y });
				if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity)) {
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
			case EntityType::SPIDER: {
				entity = new SpiderEntity(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::ENEMY: {
				entity = new EnemyEntity(AEVec2{ en.x, en.y });
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

	void WriteToFile(const char* filename, std::vector<SerializedEntity> const& entities) {
		std::ofstream out{ filename };
		for (SerializedEntity const& en : entities) {
			en.operator<<(out);
		}

		out.close();
	}

	std::vector<SerializedEntity> SerializeAll(std::vector<BaseEntity*> const& scene_entities) {
		std::vector<SerializedEntity> vect;
		for (BaseEntity* en : scene_entities) {
			if (en->layer == BaseEntity::RenderLayer::UI)
				continue;

			if (dynamic_cast<BaseUI*>(en))
				continue;

			if (dynamic_cast<Weapon*>(en))
				continue;

			vect.push_back(Serialize(en));
		}
		return vect;
	}

	std::vector<SerializedEntity> LoadFromFile(const char* filename) {
		std::vector<SerializedEntity> vect;
		std::ifstream in{ filename };
		SerializedEntity e;
		while (e.operator>>(in)) {
			SerializedEntity copy{ e };
			vect.push_back(copy);
		}
		return vect;
	}
}