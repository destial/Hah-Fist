#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include <iostream>
#include <cstdio>

Player::Player(AEVec2 pos) : BaseEntity(pos) {
	sprite = new SpriteAnimation("Assets/67.png", 7, 5);
	animationTimer = 0.f;
	animationFrame = 1.f / (7.f * 5.f);
	currentRow = currentCol = 0;
	scale = { 5.f,5.f };
	jumpHeight = 1.5f;
	jumpVelocity = sqrtf(jumpHeight * 2.f * -pBody->gravity.y);
}

Player::Player() : BaseEntity() {
	sprite = new SpriteAnimation("Assets/67.png", 7, 5);
	pBody = new PhysicsBody();
	animationTimer = 0.f;
	animationFrame = 1.f / (7.f * 5.f);
	currentRow = currentCol = 0;
	scale = { 5.f,5.f };
	jumpHeight = 50.f;
	//jumpVel = Mathf.Sqrt(maxHeightJump * 2f * -Physics.gravity.y); 
	// Calculates the jump force required to reach the maxHeightJump, mgh = 0.5mv^2 (Kinetic energy required to reach max potential energy)
	jumpVelocity = sqrtf(jumpHeight * 2.f * -pBody->gravity.y);
}

Player::~Player() {
	std::printf("Called Player deconstructor\n");
	delete sprite;
	delete pBody;
}

void Player::PreUpdate(const f32& dt) {
	BaseEntity::PreUpdate(dt);
	//this->velocity = { 0.f, 0.f };
}

void Player::Update(const f32& dt) {
	BaseEntity::Update(dt);
	
	// Out of bounds checking
	AEVec2 dir{};
	if (AEInputCheckCurr(AEVK_W)) {
		dir += { 0.f, 1.f };
		
		//velocity.y += dt * 5.f;
	}
	if (AEInputCheckCurr(AEVK_S)) {
		dir += { 0.f , -1.f };
		//velocity.y -= dt * 5.f;
	}
	if (AEInputCheckCurr(AEVK_A)) {
		dir += { -1.f , 0.f };
		//velocity.x -= dt * 5.f;
	}
	if (AEInputCheckCurr(AEVK_D)) {
		dir += { 1.f , 0.f };
		//velocity.x += dt * 5.f;
	}
	if (dir.x || dir.y) {
		AEVec2Normalize(&dir, &dir);
	}
	velocity = dir * 10.f;
	pBody->ApplyGravity(this->velocity, dt);
	if (AEInputCheckTriggered(AEVK_SPACE)) {
		velocity.y = jumpVelocity;
		std::cout << "Jump: " << velocity << std::endl;
		//pBody->state = PhysicsBody::IN_AIR;
	}
	std::cout << "velocity: " << velocity << std::endl;
	/*if (position.x > ) {
		position.x = GetWorldWidth() - scale.x;
	}
	else if (position.x < 0) {
		position.x = 0;
	}*/
	



	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 5) {
			currentCol = 0;
			if (++currentRow >= 7) {
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