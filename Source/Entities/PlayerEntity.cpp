#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include <iostream>
#include <cstdio>

Player::Player(AEVec2 pos) : GameObjectEntity(pos) {
	sprite = nullptr;
	texture = AssetManager::GetTexture("Assets/PlanetTexture.png");
	mesh = MeshRenderer::GetLeftBottomCornerRect();
	pBody = new PhysicsBody();
	animationTimer = 0.f;
	animationFrame = 1.f / (7.f * 5.f);
	currentRow = currentCol = 0;
	scale = { 5.f,5.f };
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
	BaseEntity::PreUpdate(dt);
	this->velocity = { 0.f, velocity.y };
}

void Player::Update(const f32& dt) {
	BaseEntity::Update(dt);
	
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
	velocity.x = dir.x * speed;
	
	if (AEInputCheckCurr(AEVK_SPACE) && velocity.y == 0) {
		velocity.y = jumpVelocity;
	}
	



	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 5) {
			currentCol = 0;
			if (++currentRow >= 7) {
				currentRow = 0;
			}
		}
	}
	pBody->ApplyGravity(this->velocity, dt);
}

void Player::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);
}

void Player::Render() {
	//sprite->Render(transform, currentRow, currentCol);
	GameObjectEntity::Render();
}