#pragma once
#ifndef __SERIALIZATIONMANAGER_H
#define __SERIALIZATIONMANAGER_H
#include <iostream>
#include <fstream>
#include <vector>
#include "../Entities/BaseEntity.hpp"

namespace Serialization {
	enum class EntityType : int {
		PLATFORM = 0, PLAYER, ENEMY, TROOPER, SPIDER, WALL,PROJECTILE_ENTITY, CRATE, COIN, BOSS_SPAWN_TRIGGER, TITAN_BOSS
	};

	inline std::istream& operator>> (std::istream& lhs, EntityType& et) {
		int i;
		lhs >> i;
		et = static_cast<EntityType>(i);
		return lhs;
	}

	inline std::ostream& operator<< (std::ostream& lhs, EntityType const& et) {
		int i = static_cast<int>(et);
		return lhs << i;
	}

	struct SerializedEntity {
		f32 x, y;
		f32 scale_x, scale_y, rotation;
		f32 health, damage, mass, max_health;
		EntityType type;
		int go_type, shape_type;
		// TODO: add more serialization options if needed

		SerializedEntity(); // Ctor
		SerializedEntity(SerializedEntity const&) = default; // Synthesized copy ctor
		~SerializedEntity() = default; // Synthesized dtor
		SerializedEntity& operator=(SerializedEntity const&) = default; // Synthesized copy assignment

		std::ostream& operator<< (std::ostream& rhs) const;
		std::istream& operator>> (std::istream& rhs);
	};

	SerializedEntity Serialize(BaseEntity* p);
	BaseEntity* Unserialize(SerializedEntity const& en);
	std::vector<SerializedEntity> SerializeAll(std::vector<BaseEntity*> const& scene_entities);

	void WriteToFile(const char* filename, std::vector<SerializedEntity> const& entities);
	std::vector<SerializedEntity> LoadFromFile(const char* filename);
}

#endif