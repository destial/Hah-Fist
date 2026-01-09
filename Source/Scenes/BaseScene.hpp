#pragma once
#ifndef __BASESCENE_H
#define __BASESCENE_H
#include <vector>
#include "AETypes.h"
#include "../Entities/BaseEntity.hpp"

class BaseScene {
protected:
	std::vector<BaseEntity> scene_entities;

public:
	virtual void Update(const f32& dt) = 0;
	virtual void Render() = 0;
};

#endif