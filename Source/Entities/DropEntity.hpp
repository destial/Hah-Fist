#pragma once
#ifndef __DROP_ENTITY_H
#define __DROP_ENTITY_H
#include "GameObjectEntity.hpp"
#include "../Items/SpriteSheet.hpp"
class DropEntity : public GameObjectEntity {
protected:
	f32 animationTimer, animationFrame; // Sprite animation data
	int currentRow, currentCol, columns, rows; // Sprite animation data
	SpriteSheet* sprite; // Sprite animation data
public:
	DropEntity(AEVec2 pos = { 0.f,0.f }, f32 mass = 1.f, CollisionShape shape = CollisionShape::AABB);
	~DropEntity();

	void Update(const f32& dt);
	void Render();

	void OnCollide(GameObjectEntity* go);
	virtual void OnPickup(GameObjectEntity* pickupper);
};
#endif