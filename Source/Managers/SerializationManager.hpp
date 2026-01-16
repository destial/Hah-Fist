#pragma once
#ifndef __SERIALIZATIONMANAGER_H
#define __SERIALIZATIONMANAGER_H
#include <iostream>
#include <fstream>
#include <vector>
#include "../Entities/PlayerEntity.hpp"

typedef struct SerializedEntity SerializedEntity;

namespace Serialization {
	SerializedEntity* SerializePlayer(Player* p);

	void WriteToFile(const char* filename, std::vector<SerializedEntity*> entities);
	std::vector<SerializedEntity*> LoadFromFile(const char* filename);
	void LoadToMemory(std::vector<BaseEntity*>& scene_entities, std::vector<SerializedEntity*>& serialized);
}

#endif