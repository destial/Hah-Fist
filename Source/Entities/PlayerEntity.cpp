#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"

Player::Player(AEVec2 pos) : BaseEntity(pos) {
	sprite = new SpriteAnimation("Assets/blue_portal_sprite.png", 4, 6);
	animationTimer = 0.f;
	animationFrame = 1.f / (4.f * 6.f);
	currentRow = currentCol = 0;
}

Player::~Player() {
	delete sprite;
}

void Player::PreUpdate(const f32& dt) {
	BaseEntity::PreUpdate(dt);
	this->velocity = { 0.f, 0.f };
}

void Player::Update(const f32& dt) {
	BaseEntity::Update(dt);

	if (AEInputCheckCurr(AEVK_W)) {
		velocity.y += dt * 5.f;
	}
	if (AEInputCheckCurr(AEVK_S)) {
		velocity.y -= dt * 5.f;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		velocity.x -= dt * 5.f;
	}
	if (AEInputCheckCurr(AEVK_D)) {
		velocity.x += dt * 5.f;
	}
	
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 6) {
			currentCol = 0;
			if (++currentRow >= 4) {
				currentRow = 0;
			}
		}
	}
}

void Player::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);
}

void Player::Render() {
	sprite->Render(transform, currentRow, currentCol);
}