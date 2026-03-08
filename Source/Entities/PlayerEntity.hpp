#pragma once
#ifndef __PLAYERENTITY_H
#define __PLAYERENTITY_H
#include "GameObjectEntity.hpp"
#include "../Items/SpriteSheet.hpp"
#include "../Events/InputEvent.hpp"
#include "../Entities/WeaponEntity.hpp"
#include <vector>

class Player : public GameObjectEntity {
protected:
	f32 animationTimer, animationFrame;
	int currentRow, currentCol;
	SpriteSheet* sprite;
	f32 jumpHeight;
	f32 jumpVelocity;
	f32 speed;
	std::vector<Weapon*> weapons;
public:
	Player(AEVec2 pos = { 0.f, 0.f });
	virtual ~Player();
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void OnCollide(GameObjectEntity* go);

	void AddWeapon(Weapon* weapon);
	void SwitchWeapon(int index);
};

#endif