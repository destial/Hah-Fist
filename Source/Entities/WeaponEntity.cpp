#include "WeaponEntity.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/Utils.hpp"
#include "../Events/InputEvent.hpp"
#include "../Managers/AssetManager.hpp"
#include "../UI/Debug.hpp"
#include "../Managers/CameraManager.hpp"

Weapon::Weapon(AEVec2 pos, GameObjectEntity* player) : GameObjectEntity(pos) {
	player_entity = player;
	go_type = PhysicsType::TRIGGER;
	image = AssetManager::GetTexture(ASSET_TURBOFIST_IMAGE);
	scale = { 2.5f, 2.5f };
	mesh = MeshRenderer::GetCenterRectMesh();
	layer = RenderLayer::PLAYER;
}

Weapon::~Weapon() {
	std::printf("Called Weapon deconstructor\n");
}

void Weapon::PreUpdate(const f32& dt) {
	GameObjectEntity::PreUpdate(dt);
	AEVec2 attack_direction = GetAttackDirection();
	AEVec2 right = { 1.f, 0 };
	rotation = AEVec2AngleCCW(&right, &attack_direction);
	this->position = player_entity->position + attack_direction * 2.0f;
	if (this->position.x < player_entity->position.x)
	{
		this->scale.y = -2.5f;
	}
	else
	{
		this->scale.y = 2.5f;
	}
}

void Weapon::Update(const f32& dt) {
	GameObjectEntity::Update(dt);

	if (cd_timer > 0.0f)
	{
		cd_timer -= dt;
	}
	else
	{
		if (AEInputCheckTriggered(AEVK_LBUTTON))
		{
			if (!weaponChannels)
			{
				Attack();
				cd_timer = cd_duration;
			}
			else if (!channelling)
			{
				channelling = true;
			}
		}
		else if (channelling)
		{
			if (AEInputCheckReleased(AEVK_LBUTTON)) {
				channelling = false;
				Attack();
				cd_timer = cd_duration;
			}
			else {
				channel_timer = AEClamp(channel_timer + dt, 0.0f, max_channel_time);
				CameraManager::GetInstance()->Shake(0.1f, 1.5f * channel_timer / max_channel_time);
			}
		}
	}
}

void Weapon::PostUpdate(const f32& dt) {
	GameObjectEntity::PostUpdate(dt);
}

void Weapon::Render() {
	GameObjectEntity::Render();

	auto corners = Utils::GetCorners(this);
	DebugUtils::RenderLine(corners[0], corners[1], { 255, 255, 0, 0 });
	DebugUtils::RenderLine(corners[1], corners[2], { 255, 0, 255, 0 });
	DebugUtils::RenderLine(corners[2], corners[3], { 255, 0, 0, 255 });
	DebugUtils::RenderLine(corners[3], corners[0], { 255, 255, 255, 0 });
}

void Weapon::SetPlayerEntity(GameObjectEntity* player)
{
	player_entity = player;
}

void Weapon::ResetWeapon()
{
	channel_timer = 0.0f;
	channelling = false;
	weapon_direction = { 0.f, 0.f };
}

AEVec2 Weapon::GetAttackDirection()
{
	AEVec2 attack_direction = Utils::GetMouseWorld(true) - player_entity->position;
	AEVec2Normalize(&attack_direction, &attack_direction);
	return attack_direction;
}

f32 Weapon::GetChannelTimer() const {
	return channel_timer;
}

f32 Weapon::GetMaxChannelTime() const {
	return max_channel_time;
}

f32 Weapon::GetCooldownTimer() const {
	return cd_timer;
}

f32 Weapon::GetCooldownDuration() const {
	return cd_duration;
}
