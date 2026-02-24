#include "TurboFistEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../UI/Debug.hpp"

TurboFistWeapon::TurboFistWeapon(AEVec2 pos, GameObjectEntity* player) : Weapon(pos, player)
{
	weaponChannels = true;
	player_original_mass = player->pBody->mass;
}

TurboFistWeapon::~TurboFistWeapon()
{

}

void TurboFistWeapon::Update(const f32& dt)
{
	Weapon::Update(dt);
	if (dash_timer > 0.0f)
	{
		dash_timer -= dt;
	}
	else if (player_entity->pBody->mass != player_original_mass)
	{
		player_entity->pBody->mass = player_original_mass;
		player_entity->frictionMultiplier = 1.0;
	}
}

void TurboFistWeapon::Render()
{
	Weapon::Render();
	if (channelling)
	{
		AEVec2 normalizedDirection{ weapon_direction };
		AEVec2Normalize(&normalizedDirection, &normalizedDirection);
		DebugUtils::RenderLine(position, position + (position - player_entity->position) * GetCurrentAttackStrength() * 3.0);
	}
}

void TurboFistWeapon::OnCollide(GameObjectEntity* go)
{
	if (go == player_entity)
	{
		return;
	}
	if (dash_timer > 0.0f)
	{
		go->color = { 255, 0, 0, 0 };
	}
}

void TurboFistWeapon::Attack()
{
	float attack_strength = GetCurrentAttackStrength();
	channel_timer = 0.0f;

	f32 cam_x, cam_y;
	AEGfxGetCamPosition(&cam_x, &cam_y);
	AEVec2 cam_pos = Utils::ScreenToScale(cam_x, cam_y);
	AEVec2 attack_direction = Utils::GetMouseWorld() + cam_pos - player_entity->position;
	AEVec2Normalize(&attack_direction, &attack_direction);

	dash_timer = max_dash_time;
	player_entity->velocity = attack_direction * 20 * attack_strength;
	player_entity->pBody->mass = 10.0f * attack_strength;
	player_entity->frictionMultiplier = 0.25;
}

float TurboFistWeapon::GetCurrentAttackStrength()
{
	return 1.0f + (2.0f * (static_cast<f32>(std::trunc(channel_timer)) / max_channel_time));
}