#pragma once
#ifndef __BASEENTITY_H
#define __BASEENTITY_H
#include <vector>
#include <map>
#include <functional>
#include "AEEngine.h"
#include "../Utils/Utils.hpp"
#include "../Items/Image.hpp"

class BaseEntity {
protected:
	BaseEntity(AEVec2 position = { 0.f, 0.f });
	AEMtx33 transform;
	
public:
	virtual ~BaseEntity();
	AEGfxVertexList* mesh;
	Image* image;
	AEVec2 position;
	AEVec2 velocity;
	AEVec2 scale;
	f32 rotation;
	Color color;
	int layer;

	std::map<void*, std::vector<std::function<void()>>> preupdate_listeners;
	std::map<void*, std::vector<std::function<void()>>> update_listeners;
	std::map<void*, std::vector<std::function<void()>>> postupdate_listeners;

	virtual void PreUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void PostUpdate(const f32& dt);
	virtual void Render();

	void AddUpdateListener(void* owner, std::function<void()> func);
	void AddPreUpdateListener(void* owner, std::function<void()> func);
	void AddPostUpdateListener(void* owner, std::function<void()> func);

	bool RemoveUpdateListener(void* owner);
	bool RemovePreUpdateListener(void* owner);
	bool RemovePostUpdateListener(void* owner);
};

#endif