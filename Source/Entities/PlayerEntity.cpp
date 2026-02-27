#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/CameraManager.hpp"
#include <iostream>
#include <cstdio>

Player::Player(AEVec2 pos) : GameObjectEntity(pos) {
	sprite = AssetManager::GetSpriteSheet("Assets/test_sprite.png", 3, 3);
	mesh = nullptr;
	animationTimer = 0.f;
	animationFrame = 1.f / (3.f * 3.f);
	currentRow = currentCol = 0;
	scale = { 5.f * (static_cast<f32>(sprite->image->width) / sprite->image->height), 5.f };
	jumpHeight = 8.5f;
	jumpVelocity = sqrtf(jumpHeight * 2.f * abs(pBody->gravity.y));
	speed = 10.f;
	layer = RenderLayer::PLAYER;
	go_type = PhysicsType::DYNAMIC;
}

Player::~Player() {
	std::printf("Called Player deconstructor\n");
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
		dir += { -1.f, 0.f };
	}
	if (AEInputCheckCurr(AEVK_D)) {
		dir += { 1.f, 0.f };
	}
	if (dir.x || dir.y) {
		AEVec2Normalize(&dir, &dir);
	}
	if (dir.x)
	{
		f32 spd = velocity.y == 0 ? speed : speed * 0.75f;
		velocity.x = dir.x * spd;
	}


	if (AEInputCheckCurr(AEVK_SPACE) && abs(velocity.y) == 0) {
		velocity.y += jumpVelocity;
	}
	//Testing Camera Function
	if (AEInputCheckCurr(AEVK_R)) {
		CameraManager::GetInstance()->Shake(0.1f, 5.f);
	}
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
	/*f32 x, y;
	AEGfxGetCamPosition(&x, &y);
	AEGfxSetCamPosition(Utils::WorldToScreen(position.x, position.y).x - Utils::GetScreenResolution().first / 2.f, 0.f);*/
	CameraManager::GetInstance()->SetPosition(Utils::WorldToScreen(position.x, position.y).x,0);
}

void Player::Render() {
	sprite->Render(transform, color, currentRow, currentCol);
	GameObjectEntity::Render();
}

void Player::OnCollide(GameObjectEntity* go)
{
	GameObjectEntity::OnCollide(go);
}
