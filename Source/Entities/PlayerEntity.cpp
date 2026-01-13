#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"

Player::Player(AEVec2 pos) : BaseEntity(pos) {
	mesh = MeshRenderer::CreateCircle(50, 0xFFFFFFFF);
	texture = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

Player::~Player() {
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
}

void Player::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);
}