#include "SerializationManager.hpp"
#include "AEEngine.h"

typedef struct SerializedEntity {
	float x, y;
	float scale_x, scale_y;

	// TODO: add more serialization options
} SerializedEntity;

namespace Serialization {
	SerializedEntity* SerializePlayer(Player* p) {
		
		return nullptr;
	}

	void WriteToFile(const char* filename, std::vector<SerializedEntity*> entities) {
	}

	std::vector<SerializedEntity*> LoadFromFile(const char* filename) {
		std::vector<SerializedEntity*> vect;
		return vect;
	}

	void LoadToMemory(std::vector<BaseEntity*>& scene_entities, std::vector<SerializedEntity*>& serialized) {

	}
}