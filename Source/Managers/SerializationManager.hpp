#pragma once
#ifndef __SERIALIZATIONMANAGER_H
#define __SERIALIZATIONMANAGER_H
#include <iostream>
#include <fstream>
#include <vector>
#include "../Entities/BaseEntity.hpp"

namespace Serialization {
	enum class EntityType : int {
		PLATFORM = 0, PLAYER, ENEMY, TROOPER
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
		f32 health, damage, mass;
		EntityType type;
		// TODO: add more serialization options if needed

		SerializedEntity();

		SerializedEntity& operator=(SerializedEntity const& rhs);
		std::ostream& operator<< (std::ostream& rhs) const;
		std::istream& operator>> (std::istream& rhs);
	};

	SerializedEntity Serialize(BaseEntity* p);
	BaseEntity* Unserialize(SerializedEntity const& en);
	std::vector<SerializedEntity> SerializeAll(std::vector<BaseEntity*> const& scene_entities);

	void WriteToFile(const char* filename, std::vector<SerializedEntity> const& entities);
	std::vector<SerializedEntity> LoadFromFile(const char* filename);
	void LoadToMemory(std::vector<BaseEntity*>& scene_entities, std::vector<SerializedEntity>& serialized);
}

#endif