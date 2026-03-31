#pragma once
#ifndef __PROJECTILE_ENTITY_H
#define __PROJECTILE_ENTITY_H

#define INITIAL_SPEED 10.f
#include "EnemyEntity.hpp"
class ProjectileEntity : public EnemyEntity {
private:
public:
	ProjectileEntity(AEVec2 pos, f32 speed = INITIAL_SPEED);
	~ProjectileEntity() override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class
	void OnIdle(const f32& dt) override;
	void OnPatrol(const f32& dt) override;
	void OnChase(const f32& dt) override;
	void OnAttack(const f32& dt) override;
	void OnDead() override;
};
#endif