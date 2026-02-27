#pragma once
#ifndef __BASESCENE_H
#define __BASESCENE_H
#include <vector>
#include <memory>
#include "AETypes.h"
#include "../Entities/BaseEntity.hpp"
#include "../Items/ParticleSystem.hpp"
#include "../Managers/PhysicsManager.hpp"
#include "../Managers/CameraManager.hpp"


class BaseScene {
protected:
	std::vector<BaseEntity*> scene_entities;
	ParticleSystem* particleSystem;
	PhysicsManager* physicsManager{nullptr};
	CameraManager* camManager;
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
	void RemoveEntityFromScene(BaseEntity* entity);

	template<typename E> E* GetFirstEntityOfType() const;
	template<typename E> std::vector<E*> GetEntitesOfType() const;

	std::vector<BaseEntity*> const& Entities() const;
};

#endif
