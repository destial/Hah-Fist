#include "BaseEntity.hpp"

BaseEntity::BaseEntity(AEVec2 position) {
	this->position = position;
	this->velocity = { 0.f, 0.f };
}

BaseEntity::~BaseEntity() {

}