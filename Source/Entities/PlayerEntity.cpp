#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"

Player::Player(AEVec2 pos) : BaseEntity(pos) {
	sprite = new SpriteAnimation("Assets/blue_portal_sprite.png", 4, 6);
	animationTimer = 0.f;
	animationFrame = 5.f / (4.f * 6.f);
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
		++this->velocity.y;
	}
	if (AEInputCheckCurr(AEVK_S)) {
		--this->velocity.y;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		--this->velocity.x;
	}
	if (AEInputCheckCurr(AEVK_D)) {
		++this->velocity.x;
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