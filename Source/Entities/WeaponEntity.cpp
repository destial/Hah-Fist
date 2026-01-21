#include "WeaponEntity.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Events/InputEvent.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"


Weapon::Weapon(AEVec2 pos, BaseEntity* player) : BaseEntity(pos) {
	player_entity = player;

	texture = AssetManager::GetTexture("Assets/PlanetTexture.png");
	mesh = MeshRenderer::GetLeftBottomCornerRect();
	layer = 2;
}

Weapon::~Weapon() {
	std::printf("Called Weapon deconstructor\n");
}

void Weapon::PreUpdate(const f32& dt) {
	BaseEntity::PreUpdate(dt);
	this->position = player_entity->position + player_entity->scale * 0.5;
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
	s32 x, y;
	AEInputGetCursorPosition(&x, &y);

	AEVec2 attack_direction = Utils::Screen_To_World(x, y) - player_entity->position;
	AEVec2Normalize(&attack_direction, &attack_direction);

	dash_timer = max_dash_time;
	weapon_direction = attack_direction * 60 * attack_strength;

}