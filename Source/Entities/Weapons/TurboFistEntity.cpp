#include "TurboFistEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../UI/Debug.hpp"
#include "../../Managers/AssetManager.hpp"
#include "../Projectiles/BaseProjectile.hpp"


TurboFistWeapon::TurboFistWeapon(AEVec2 pos, GameObjectEntity* player) : Weapon(pos, player)
{
	weaponChannels = true;
	image = AssetManager::GetTexture("Assets/fist1.png");
	player_original_mass = player->pBody->mass;
	max_channel_time = 1.0f;
	cd_duration = 0.5f;
	damage = 0.01f;
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
		if (go->entity_type == EntityType::ENEMY)
		{
			go->health -= damage * AEVec2Length(&player_entity->velocity);
			//go->velocity += player_entity->velocity * 1.25;
			dash_timer = 0;
		}
		else if (go->entity_type == EntityType::PROJECTILE)
		{
			BaseProjectile* e = dynamic_cast<BaseProjectile*>(go);
			
			//i think this is correct, not sure
			if (e->TryChangeOwnership(player_entity))
			{
				go->velocity = player_entity->velocity * 1.25f;
			}
		}
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