#include "TurboFistEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../UI/Debug.hpp"
#include "../../Managers/AssetManager.hpp"

TurboFistWeapon::TurboFistWeapon(AEVec2 pos, GameObjectEntity* player) : Weapon(pos, player)
{
	weaponChannels = true;
	image = AssetManager::GetTexture("Assets/fist1.png");
	player_original_mass = player->pBody->mass;
}

TurboFistWeapon::~TurboFistWeapon()
{

}

void TurboFistWeapon::Update(const f32& dt)
{
	if (!isActive)
	{
		return;
	}
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

	AEVec2 attack_direction = GetAttackDirection();

	dash_timer = max_dash_time;
	player_entity->velocity = attack_direction * 20 * attack_strength;
	player_entity->pBody->mass = 10.0f * attack_strength;
	player_entity->frictionMultiplier = 0.25;
}

float TurboFistWeapon::GetCurrentAttackStrength()
{
	return 1.0f + (2.0f * (static_cast<f32>(std::trunc(channel_timer)) / max_channel_time));
}

void TurboFistWeapon::ResetWeapon()
{
	Weapon::ResetWeapon();
	dash_timer = 0.0f;
	dashing = false;
}