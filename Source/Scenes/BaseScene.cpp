#include "BaseScene.hpp"

BaseScene::BaseScene() : scene_entities(0) {
}

BaseScene::~BaseScene() {

}

void BaseScene::Render() {
	for (BaseEntity* entity : scene_entities) {
		entity->Render();
	}
}