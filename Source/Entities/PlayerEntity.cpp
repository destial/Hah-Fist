#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include <iostream>
#include <cstdio>

Player::Player(AEVec2 pos) : GameObjectEntity(pos) {
	sprite = AssetManager::GetSpriteSheet("Assets/test_sprite.png", 3, 3);
	mesh = MeshRenderer::GetCenterRectMesh();
	pBody = new PhysicsBody();
	animationTimer = 0.f;
	animationFrame = 1.f / (3.f * 3.f);
	currentRow = currentCol = 0;
	scale = { 5.f * (static_cast<f32>(sprite->image->width) / sprite->image->height), 5.f };
	jumpHeight = 2.f;
	jumpVelocity = sqrtf(jumpHeight * 2.f * -pBody->gravity.y);
	speed = 10.f;
	layer = 2;
}

Player::~Player() {
	std::printf("Called Player deconstructor\n");
	delete pBody;
}

void Player::PreUpdate(const f32& dt) {
	GameObjectEntity::PreUpdate(dt);
	color = { 255, 255, 255, 255 };
}

void Player::Update(const f32& dt) {
	GameObjectEntity::Update(dt);
	// Out of bounds checking
	AEVec2 dir{};
	if (AEInputCheckCurr(AEVK_A)) {
		dir += { -1.f , 0.f };
	}
	if (AEInputCheckCurr(AEVK_D)) {
		dir += { 1.f , 0.f };
	}
	if (dir.x || dir.y) {
		AEVec2Normalize(&dir, &dir);
	}
	velocity.x += dir.x * speed;
	
	if (AEInputCheckCurr(AEVK_SPACE) && velocity.y == 0) {
		velocity.y += jumpVelocity;
	}
	pBody->ApplyGravity(this->velocity, dt);
}

void Player::PostUpdate(const f32& dt) {
	GameObjectEntity::PostUpdate(dt);
	currentRow = 0;
	if (velocity.x > 0) {
		currentRow = 1;
	}
	if (velocity.x < 0) {
		currentRow = 2;
	}
	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 3) {
			currentCol = 0;
		}
	}
}

void Player::Render() {
	sprite->Render(transform, currentRow, currentCol);
	//GameObjectEntity::Render();
}