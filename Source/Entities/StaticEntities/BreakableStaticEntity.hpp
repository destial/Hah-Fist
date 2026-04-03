#pragma once
#ifndef __BREAKABLE_STATIC_ENTITY_H
#define __BREAKABLE_STATIC_ENTITY_H
#include "../StaticEntity.hpp"
#include "../../Items/SpriteSheet.hpp"

class BreakableStaticEntity : public StaticEntity {
protected:
	f32 animation_timer, animation_frame; // Sprite animation data
	int current_row, current_col, columns, rows; // Sprite animation data
	SpriteSheet* sprite; // Sprite animation data
	SpriteSheet* breaking_sprite; // Sprite animation data
public:
	BreakableStaticEntity(AEVec2 pos = { 0.f,0.f }, std::string file_name = "Assets/coin.png", int _columns = 20, int _rows = 1);
	virtual ~BreakableStaticEntity();

	virtual void Render();
	virtual void Update(const f32& dt);
};
#endif