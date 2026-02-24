#include "GrappleFistEntity.hpp"
#include "../../Utils/AEOverload.hpp"

GrappleFistWeapon::GrappleFistWeapon(AEVec2 pos, GameObjectEntity* player) : Weapon(pos, player)
{
	weaponChannels = false;
}
GrappleFistWeapon::~GrappleFistWeapon()
{

}

void GrappleFistWeapon::PreUpdate(const f32& dt)
{
	if (grappleState == INACTIVE)
	{
		Weapon::PreUpdate(dt);
	}
}

void GrappleFistWeapon::Update(const f32& dt)
{
	Weapon::Update(dt);
	AEVec2 travel_direction = position - player_entity->position;
	if (grappleState == SHOOTING)
	{
		travelDuration += dt;
		position += snapshottedAttackDirection * dt * grappleSpeedMult;
		if (travelDuration >= 1.0f || AEVec2Length(&travel_direction) >= 20.0f)
		{
			travelDuration = 0.0f;
			grappleState = RETURNING;
		}
		return;
	}
	else if (grappleState == RETURNING)
	{
		if (AEVec2Length(&travel_direction) <= 3.0f)
		{
			grappleState = INACTIVE;
		}
		AEVec2Normalize(&travel_direction, &travel_direction);
		position -= travel_direction * dt * grappleSpeedMult;
		return;
	}
	else if (grappledObject != nullptr)
	{
		if (AEVec2Length(&travel_direction) <= 3.0f)
		{
			grappledObject = nullptr;
			grappleState = INACTIVE;
			return;
		}
		AEVec2Normalize(&travel_direction, &travel_direction);
		if (grappleState == HOOKING_OBJECT_TO_PLAYER)
		{
			position -= travel_direction * dt * grappleSpeedMult;
			grappledObject->position = position;
		}
		else if(grappleState == HOOKING_PLAYER_TO_STATIC_OBJECT)
		{
			player_entity->velocity = travel_direction * grappleSpeedMult;
		}
		return;
	}
}

void GrappleFistWeapon::Render()
{
	Weapon::Render();
}

void GrappleFistWeapon::OnCollide(GameObjectEntity* go)
{
	if (go->go_type == GameObjectEntity::TRIGGER)
	{
		return;
	}
	if (go == player_entity)
	{
		return;
	}
	if (grappleState == SHOOTING || grappleState == RETURNING)
	{
		grappledObject = go;
		if (go->go_type == GameObjectEntity::DYNAMIC)
		{
			grappleState = HOOKING_OBJECT_TO_PLAYER;
		}
		else
		{
			grappleState = HOOKING_PLAYER_TO_STATIC_OBJECT;
		}
	}
}

void GrappleFistWeapon::Attack()
{
	grappleState = SHOOTING;
	snapshottedAttackDirection = GetAttackDirection();
}