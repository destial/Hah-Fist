#include <fstream>
#include <iostream>
#include "SerializationManager.hpp"
#include "AEEngine.h"
#include "../Entities/GameObjectEntity.hpp"
#include "../Entities/StaticEntity.hpp"
#include "../Entities/StaticEntities/BreakableCrateEntity.hpp"
#include "../Entities/StaticEntities/BossSpawnTriggerEntity.hpp"
#include "../Entities/DropEntities/CoinEntity.hpp"
#include "../Entities/PlayerEntity.hpp"
#include "../Entities/WeaponEntity.hpp"
#include "../Entities/Enemies/EnemyEntity.hpp"
#include "../Entities/Enemies/TrooperEntity.hpp"
#include "../Entities/Enemies/SpiderEntity.hpp"
#include "../Entities/Enemies/ProjectileEntity.hpp"
#include "../Entities/Enemies/TitanEntity.hpp"
#include "../Entities/Enemies/PayloadEntity.hpp"
#include "../Entities/Enemies/IronsideEntity.hpp"
#include "../UI/BaseUI.hpp"
#include "../Utils/MeshRenderer.hpp"

namespace Serialization {

	SerializedEntity::SerializedEntity()
	: x{ 0 }, y{ 0 }, scale_x{ 0 }, scale_y{ 0 },
	  rotation{ 0 }, health{ 0 }, damage{ 0 }, 
	  mass{ 0 }, max_health{ 0 }, type{ EntityType::PLATFORM },
	  go_type{ -1 }, shape_type{ -1 } {
	}

	std::ostream& SerializedEntity::operator<< (std::ostream& rhs) const {
		return rhs << x << ',' << y << ',' << scale_x << ','
			<< scale_y << ',' << rotation << ',' << health
			<< ',' << damage << ',' << mass << ',' << type << ',' << max_health 
			<< ',' << go_type << ',' << shape_type << '|';
	}

	std::istream& SerializedEntity::operator>> (std::istream& rhs) {
		char c;
		rhs >> x >> c >> y >> c >> scale_x >> c >> scale_y
			>> c >> rotation >> c >> health >> c >> damage >> c
			>> mass >> c >> type >> c;
		if (c == '|') {
			max_health = health;
			return rhs;
		}
		
		rhs >> max_health >> c;
		if (c == '|') {
			return rhs;
		}

		rhs >> go_type >> c;
		if (c == '|') {
			return rhs;
		}

		rhs >> shape_type >> c;
		return rhs;
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
			s.max_health = go->max_health;
			s.damage = go->damage;
			s.type = EntityType::PLATFORM;
			s.mass = go->pBody->mass;
			s.go_type = static_cast<int>(go->go_type);
			s.shape_type = static_cast<int>(go->shape);
			if (BreakableCrateEntity* be = dynamic_cast<BreakableCrateEntity*>(go)) {
				s.type = EntityType::CRATE;
			}
			else if (CoinEntity* ce = dynamic_cast<CoinEntity*>(go)) {
				s.type = EntityType::COIN;
			}
			else if (BossSpawnTriggerEntity* bste = dynamic_cast<BossSpawnTriggerEntity*>(go))
			{
				s.type = EntityType::BOSS_SPAWN_TRIGGER;
			}
			else if (StaticEntity* se = dynamic_cast<StaticEntity*>(en)) {
				if (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_WALL) {
					s.type = EntityType::WALL;
				}
				else if (se->GetStaticType() == StaticEntity::STATIC_TYPE::TYPE_PLATFORM) {
					s.type = EntityType::PLATFORM;
				}
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
				else if (ProjectileEntity* pEntity = dynamic_cast<ProjectileEntity*>(enemy)) {
					s.type = EntityType::PROJECTILE_ENTITY;
				}
				else if (TitanEntity* tEnt = dynamic_cast<TitanEntity*>(enemy))
				{
					s.type = EntityType::TITAN_BOSS;
				}
				else if (PayloadEntity* pEnt = dynamic_cast<PayloadEntity*>(enemy))
				{
					s.type = EntityType::PAYLOAD_BOSS;
				}
				else if (IronsideEntity* iEnt = dynamic_cast<IronsideEntity*>(enemy))
				{
					s.type = EntityType::IRONSIDE_BOSS;
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
				entity->mesh = MeshRenderer::GetCenterRectMesh();
				break;
			}
			case EntityType::WALL: {
				entity = new StaticEntity(StaticEntity::STATIC_TYPE::TYPE_WALL,AEVec2{ en.x, en.y });
				entity->mesh = MeshRenderer::GetCenterRectMesh();
				break;
			}
			case EntityType::CRATE: {
				entity = new BreakableCrateEntity(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::COIN: {
				entity = new CoinEntity(AEVec2{ en.x, en.y });
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
			case EntityType::PROJECTILE_ENTITY: {
				entity = new ProjectileEntity(AEVec2{ en.x,en.y });
				break;
			}
			case EntityType::ENEMY: {
				entity = new EnemyEntity(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::BOSS_SPAWN_TRIGGER: {
				entity = new BossSpawnTriggerEntity(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::TITAN_BOSS: {
				entity = new TitanEntity(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::PAYLOAD_BOSS: {
				entity = new PayloadEntity(AEVec2{ en.x, en.y });
				break;
			}
			case EntityType::IRONSIDE_BOSS: {
				entity = new IronsideEntity(AEVec2{ en.x, en.y });
				break;
			}
			default: break;
		}

		if (entity) {
			entity->scale = { en.scale_x, en.scale_y };
			entity->rotation = en.rotation;

			if (GameObjectEntity* go = dynamic_cast<GameObjectEntity*>(entity)) {
				/*go->health = en.health;
				go->max_health = en.max_health;*/
				//go->damage = en.damage;
				go->pBody->mass = en.mass;

				if (en.go_type != -1) {
					go->go_type = static_cast<GameObjectEntity::PhysicsType>(en.go_type);
				}

				if (en.shape_type != -1) {
					go->shape = static_cast<GameObjectEntity::CollisionShape>(en.shape_type);
				}
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
			if (en->layer == BaseEntity::RenderLayer::UI || en->layer == BaseEntity::RenderLayer::BACKGROUND ||
				en->layer == BaseEntity::RenderLayer::WORLD_UI)
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