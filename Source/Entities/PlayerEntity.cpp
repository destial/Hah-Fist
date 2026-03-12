#include "PlayerEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/CameraManager.hpp"
#include "Projectiles/BaseProjectile.hpp"
#include "../Scenes/BaseScene.hpp"  
#include "../Managers/SceneManager.hpp"
#include <iostream>
#include <cstdio>

Player::Player(AEVec2 pos) : GameObjectEntity(pos) {
	int columns{ 18 };
	int rows{ 3 };

	sprite = AssetManager::GetSpriteSheet(ASSET_PLAYER_SPRITE, rows, columns);
	mesh = nullptr;
	animationTimer = 0.f;
	animationFrame = 1.f / static_cast<f32>(columns * rows);
	currentRow = currentCol = 0;
	scale = { 5.f * ((static_cast<f32>(sprite->image->width) / static_cast<f32>(columns)) / (sprite->image->height / static_cast<f32>(rows))) , 5.f };
	jumpHeight = 8.5f;
	jumpVelocity = sqrtf(jumpHeight * 2.f * abs(pBody->gravity.y));
	speed = 10.f;
	entity_type = EntityType::PLAYER;
	layer = RenderLayer::PLAYER;
	go_type = PhysicsType::DYNAMIC;
}

Player::~Player() {
	std::printf("Called Player deconstructor\n");
	for (Weapon* wp : weapons)
	{
		SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(wp);
	}
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
		//f32 spd = velocity.y == 0 ? speed : speed * pBody->air_strength * 0.75f;
		if (!(dir.x < 0 && velocity.x < 0 || dir.x > 0 && velocity.x > 0))
		{
			velocity.x = 0.f;
		}
		if (abs(velocity.x) < speed)
		{
			velocity.x += dir.x * speed;
		}
	}
	if (AEInputCheckCurr(AEVK_1)) {
		SwitchWeapon(0);
	}
	else if (AEInputCheckCurr(AEVK_2)) {
		SwitchWeapon(1);
	}
	else if (AEInputCheckCurr(AEVK_3)) {
		SwitchWeapon(2);
	}


	if (AEInputCheckCurr(AEVK_SPACE) && pBody->is_standing_above) {
		velocity.y += jumpVelocity;
	}
	//Testing Camera Function
	if (AEInputCheckCurr(AEVK_R)) {
		CameraManager::GetInstance()->Shake(0.1f, 5.f);

	}
	//Testing Shooting Function
	if (AEInputCheckCurr(AEVK_T) && abs(velocity.y) == 0) {
		velocity.y += jumpVelocity * 4.0f;
		//Have to offset so it does not instantly collide and delete its own projectile
		AEVec2 playerPosOff{position.x + dir.x + 2.f,position.y};
		AEVec2 shootDir{ dir.x, 0.f };
		AEVec2Normalize(&shootDir, &shootDir);
		f32 bulletSpeed = 50.f;
		f32 bulletDamage = 25.f;
		BaseProjectile* bullet = new BaseProjectile(playerPosOff, shootDir, bulletSpeed, bulletDamage, this);
		SceneManager::GetInstance()->GetCurrentScene()->AddEntityToScene(bullet);

	}
}

void Player::PostUpdate(const f32& dt) {
	GameObjectEntity::PostUpdate(dt);
	if (isActive) {

	}
	currentRow = 1;
	if (velocity.x != 0) {
		currentRow = 0;
		if (velocity.x < 0)
		{
			if (this->scale.x > 0)
			{
				this->scale.x *= -1;
			}
		}
		else
		{
			if (this->scale.x < 0)
			{
				this->scale.x *= -1;
			}
		}
		if (AEVec2Length(&velocity) > 50.0f)
		{
			currentRow = 2;
		}
	}
	

	if ((animationTimer += dt) > animationFrame) {
		animationTimer = 0.f;
		if (++currentCol >= 32) {
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
	f32 p_health = health;
	GameObjectEntity::OnCollide(go);
	if (p_health < health) {
		AEAudioPlay(AssetManager::GetAudio(ASSET_PLAYERHURT_AUDIO), Game::GetSfxGroup(), 1.f, 1.f, 0);
	}
}

void Player::AddWeapon(Weapon* weapon)
{
	weapons.push_back(weapon);
}

void Player::SwitchWeapon(int index)
{
	if (weapons.size() <= index) { return; }

	for (int i{ 0 }; i < weapons.size(); ++i)
	{
		if (i != index)
		{
			weapons[i]->ResetWeapon();
		}
		weapons[i]->isActive = i == index;
	}
}

Weapon* Player::CurrentWeapon() const {
	for (int i{ 0 }; i < weapons.size(); ++i) {
		if (weapons[i]->isActive) {
			return weapons[i];
		}
	}
	return nullptr;
}

void Player::AddCoin()
{
	coinCount++;
}