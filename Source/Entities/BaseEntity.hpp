#pragma once
#ifndef __BASEENTITY_H
#define __BASEENTITY_H
#include "AETypes.h"
#include "AEMath.h"

class BaseEntity {
private:
	BaseEntity(AEVec2 position);
	~BaseEntity();
public:
	AEVec2 position;
	AEVec2 velocity;
};

#endif