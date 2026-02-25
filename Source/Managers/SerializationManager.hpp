#pragma once
#ifndef __SERIALIZATIONMANAGER_H
#define __SERIALIZATIONMANAGER_H
#include <iostream>
#include <fstream>
#include <vector>
#include "../Entities/BaseEntity.hpp"

namespace Serialization {
	struct SerializedEntity;

	SerializedEntity Serialize(BaseEntity* p);
	BaseEntity* Unserialize(SerializedEntity const& en);
	std::vector<SerializedEntity> SerializeAll(std::vector<BaseEntity*>& scene_entities);

	void WriteToFile(const char* filename, std::vector<SerializedEntity> entities);
	std::vector<SerializedEntity> LoadFromFile(const char* filename);
	void LoadToMemory(std::vector<BaseEntity*>& scene_entities, std::vector<SerializedEntity>& serialized);
}

#endif