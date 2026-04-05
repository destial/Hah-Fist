/*!
* @file BreakableStaticEntity.cpp
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 12th March 2026
* @course CSD1451
* @brief This source file declares the BreakableStaticEntity class' member functions, which are
* Destructable objects in the game.
*/
#include "BreakableStaticEntity.hpp"
#include "../../Managers/SceneManager.hpp"

BreakableStaticEntity::BreakableStaticEntity(AEVec2 pos, std::string file_name, int col, int rw) 
	: StaticEntity{ STATIC_TYPE::TYPE_WALL, pos, 1.f, CollisionShape::AABB, PhysicsType::STATIC }, columns{ col }, rows{ rw } {
	entity_type = EntityType::BREAKABLE_STATIC;
	breaking_sprite = AssetManager::GetSpriteSheet(ASSET_BREAKING_FRAME_SPRITE, 1, 4);

	sprite = AssetManager::GetSpriteSheet(file_name, rows, columns);
	mesh = nullptr;
	animation_timer = 0.f;
	animation_frame = 1.f / static_cast<f32>(columns * rows);
	current_row = current_col = 0;
	scale = { 5.f * ((static_cast<f32>(sprite->image->width) / static_cast<f32>(columns)) / (sprite->image->height / static_cast<f32>(rows))) , 5.f };
}

BreakableStaticEntity::~BreakableStaticEntity() {} // Empty dtor

void BreakableStaticEntity::Update(const f32& dt) {
	StaticEntity::Update(dt);
	if (health <= 0.f && is_active) {
		// OnBroken();
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
		return;
	}

	if ((animation_timer += dt) > animation_frame) {
		animation_timer = 0.f;
		if (++current_col >= columns) {
			current_col = 0;
		}
	}
}

void BreakableStaticEntity::Render() {
	sprite->Render(transform, color, current_row, current_col);
	breaking_sprite->Render(transform, color, 0, 4 - static_cast<int>(4.0f * (health / max_health)));
	StaticEntity::Render();
}