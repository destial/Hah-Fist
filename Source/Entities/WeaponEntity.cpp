#include "WeaponEntity.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"


Weapon::Weapon(AEVec2 pos, BaseEntity* player) : BaseEntity(pos), dash_timer(0.f), channel_timer(0.f), channelling(false), weapon_direction({ 0.f, 0.f }) {
	player_entity = player;

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
		player_entity->velocity = weapon_direction;
	}
}

void Weapon::PostUpdate(const f32& dt) {
	BaseEntity::PostUpdate(dt);
}

void Weapon::Render() {
	BaseEntity::Render();
}

void Weapon::Attack() {
	float attack_strength = channel_timer / max_channel_time;

	AEVec2 attack_direction = Utils::Get_Mouse_World() - player_entity->position;
	AEVec2Normalize(&attack_direction, &attack_direction);

	dash_timer = max_dash_time;
	weapon_direction = attack_direction * 60 * attack_strength;

}