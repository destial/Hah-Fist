#pragma once
#ifndef __BASESCENE_H
#define __BASESCENE_H
#include <vector>
#include <memory>
#include "AETypes.h"
#include "../Entities/BaseEntity.hpp"
#include "../Items/ParticleSystem.hpp"
#include "../Managers/PhysicsManager.hpp"

class BaseScene {
protected:
	std::vector<BaseEntity*> scene_entities;
	ParticleSystem* particleSystem;
	PhysicsManager* physicsManager{nullptr};
	BaseScene();

public:
	virtual ~BaseScene();
	virtual void Init() = 0;
	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
	virtual void End();

	void AddEntityToScene(BaseEntity* entity);
};

#endif
