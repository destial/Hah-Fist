#pragma once
#ifndef __BASEENTITY_H
#define __BASEENTITY_H
#include "AEEngine.h"

class BaseEntity {
protected:
	BaseEntity(AEVec2 position);
	AEGfxVertexList* mesh;
	AEMtx33 transform;
public:
	virtual ~BaseEntity();
	AEVec2 position;
	AEVec2 velocity;
	AEVec2 scale;
	f32 rotation;

	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();
};

#endif