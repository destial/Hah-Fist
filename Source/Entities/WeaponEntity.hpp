#pragma once
#ifndef __WEAPONENTITY_H
#define __WEAPONENTITY_H
#include "GameObjectEntity.hpp"


class Weapon : public GameObjectEntity {
protected:
	GameObjectEntity* player_entity;

	bool weaponChannels{ false };
	bool channelling{ false };
	f32 max_channel_time{ 2.0f };
	f32 channel_timer{ 0.0f };
	AEVec2 weapon_direction{ 0.f, 0.f };
public:
	Weapon(AEVec2 pos = { 0.f, 0.f }, GameObjectEntity* Player = nullptr);
	~Weapon();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void Attack() = 0;
};

#endif