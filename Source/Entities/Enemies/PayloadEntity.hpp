#pragma once
#ifndef __PAYLOADENTITY_H
#define __PAYLOADENTITY_H
#include "EnemyEntity.hpp"
class PayloadEntity : public EnemyEntity {
protected:
	GameObjectEntity* ground;
public:
	PayloadEntity(AEVec2 pos);
	~PayloadEntity() override;
	void PreUpdate(const f32& dt) override;
	void Update(const f32& dt) override;
	void PostUpdate(const f32& dt) override;
	void Render() override;
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class
	void OnIdle(const f32& dt) override;
	void OnPatrol(const f32& dt) override;
	void OnChase(const f32& dt) override;
	void OnStun(const f32& dt) override;
	void OnDead(const f32& dt) override;
};



#endif