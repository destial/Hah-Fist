#pragma once
#ifndef __WEAPONENTITY_H
#define __WEAPONENTITY_H
#include "BaseEntity.hpp"


class Weapon : public BaseEntity {
protected:
	BaseEntity *player_entity;
	bool channelling = false;
	bool dashing = false;
	f32 max_dash_time = 0.25f;
	f32 dash_timer = 0.0f;

	f32 max_channel_time = 0.5f;
	f32 channel_timer = 0.0f;
	AEVec2 weapon_direction;
public:
	Weapon(AEVec2 pos = { 0.f, 0.f }, BaseEntity* Player = nullptr);
	~Weapon();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	void Attack();
};

#endif