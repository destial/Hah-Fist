/*!
* @file SerializationManager.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 23 February 2026
* @course CSD1451
* @brief Declaration file for to save a level to serialized text file
*/

#pragma once
#ifndef __SERIALIZATIONMANAGER_H
#define __SERIALIZATIONMANAGER_H
#include <iostream>
#include <fstream>
#include <vector>
#include "../Entities/BaseEntity.hpp"

namespace Serialization {

	// Declare all known entity types
	enum class EntityType : int {
		PLATFORM = 0, PLAYER, ENEMY, TROOPER, SPIDER, WALL,PROJECTILE_ENTITY, CRATE, COIN, BOSS_SPAWN_TRIGGER, TITAN_BOSS,PAYLOAD_BOSS,IRONSIDE_BOSS
	};

	// Overloaded stream operators for EntityType enum
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

	/*!
	* @brief Serialized entity class to read and load from text
	*/
	struct SerializedEntity {
		f32 x, y;
		f32 scale_x, scale_y, rotation;
		f32 health, damage, mass, max_health;
		EntityType type;
		int go_type, shape_type;

		SerializedEntity(); // Ctor
		SerializedEntity(SerializedEntity const&) = default; // Synthesized copy ctor
		~SerializedEntity() = default; // Synthesized dtor
		SerializedEntity& operator=(SerializedEntity const&) = default; // Synthesized copy assignment

		// Overloaded stream operators for SerializedEntity
		std::ostream& operator<< (std::ostream& rhs) const;
		std::istream& operator>> (std::istream& rhs);
	};

	/*!
	* @brief Convert an entity to a serialized entity
	* @param p - The Base Entity
	* @return The serialized entity
	*/
	SerializedEntity Serialize(BaseEntity* p);

	/*!
	* @brief Convert a serialized entity to a scene entity
	* @param en - The serialized entity
	* @return The scene entity
	*/
	BaseEntity* Unserialize(SerializedEntity const& en);

	/*!
	* @brief Serialize a given vector of scene entities into serialized entities
	* @param scene_entities - The vector of loaded scene entities
	* @return A vector of serialized entities
	*/
	std::vector<SerializedEntity> SerializeAll(std::vector<BaseEntity*> const& scene_entities);

	/*!
	* @brief Write the converted serialized entities to level file
	* @param file_name - The file name to write to
	* @param entities - The vector of serialized entities
	*/
	void WriteToFile(const char* file_name, std::vector<SerializedEntity> const& entities);

	/*!
	* @brief Load a level file into serialized entities
	* @param filename - The file to load from
	* @return A vector of serialized entities
	*/
	std::vector<SerializedEntity> LoadFromFile(const char* file_name);
}

#endif