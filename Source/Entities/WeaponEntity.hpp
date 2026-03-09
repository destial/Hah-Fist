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
	float cd_timer{ 0.0f };
	float cd_duration{2.0f};
public:
	Weapon(AEVec2 pos = { 0.f, 0.f }, GameObjectEntity* Player = nullptr);
	~Weapon();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void Attack() = 0;

	virtual void ResetWeapon();

	void SetPlayerEntity(GameObjectEntity* player);

	AEVec2 GetAttackDirection();

	f32 GetChannelTimer() const;
	f32 GetMaxChannelTime() const;
	f32 GetCooldownTimer() const;
	f32 GetCooldownDuration() const;
};

#endif