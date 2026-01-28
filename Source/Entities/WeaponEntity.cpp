#include "WeaponEntity.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/Utils.hpp"
#include "../Events/InputEvent.hpp"
#include "../Managers/AssetManager.hpp"
#include "../UI/Debug.hpp"


Weapon::Weapon(AEVec2 pos, GameObjectEntity* player) : BaseEntity(pos), dash_timer(0.f), channel_timer(0.f), channelling(false), weapon_direction({ 0.f, 0.f }) {
	player_entity = player;
	player_original_mass = player_entity->pBody->mass;

	image = AssetManager::GetTexture("Assets/test_fist.png");
	mesh = MeshRenderer::GetCenterRectMesh();
	layer = 2;
}

Weapon::~Weapon() {
	std::printf("Called Weapon deconstructor\n");
}

void Weapon::PreUpdate(const f32& dt) {
	BaseEntity::PreUpdate(dt);
	AEVec2 attack_direction = Utils::Get_Mouse_World() - player_entity->position;
	AEVec2Normalize(&attack_direction, &attack_direction);
	AEVec2 right = { 1.f, 0 };
	rotation = AEVec2AngleCCW(&right, &attack_direction);
	this->position = player_entity->position + attack_direction;
}

void Weapon::Update(const f32& dt) {
	BaseEntity::Update(dt);
	if (!channelling && AEInputCheckTriggered(AEVK_W)) {
		channelling = true;
	}
	else if (channelling) {
		if (AEInputCheckReleased(AEVK_W)) {
			channelling = false;
			Attack();
		}
		else {
			channel_timer = AEClamp(channel_timer + dt, 0.0f, max_channel_time);
		}
	}
	if (dash_timer > 0.0f)
	{
		dash_timer -= dt;
	}
	else if (player_entity->pBody->mass != player_original_mass)
	{
		player_entity->pBody->mass = player_original_mass;
	}
}

void Weapon::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);
}

void Weapon::Render() {
	BaseEntity::Render();

	auto corners = Utils::GetCorners(this);
	DebugUtils::RenderLine(corners[0], corners[1], { 255, 255, 0, 0 });
	DebugUtils::RenderLine(corners[1], corners[2], { 255, 0, 255, 0 });
	DebugUtils::RenderLine(corners[2], corners[3], { 255, 0, 0, 255 });
	DebugUtils::RenderLine(corners[3], corners[0], { 255, 255, 255, 0 });
}

void Weapon::Attack() {
	float attack_strength = channel_timer / max_channel_time + 0.25;

	AEVec2 attack_direction = Utils::Get_Mouse_World() - player_entity->position;
	AEVec2Normalize(&attack_direction, &attack_direction);

	dash_timer = max_dash_time;
	player_entity->velocity = attack_direction * 60 * attack_strength;
	player_entity->pBody->mass = 10.0f * attack_strength;
}