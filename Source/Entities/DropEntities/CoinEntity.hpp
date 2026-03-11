#pragma once
#ifndef __COIN_ENTITY_H
#define __COIN_ENTITY_H
#include "../DropEntity.hpp"
class CoinEntity : public DropEntity {
public:
	CoinEntity(AEVec2 pos = { 0.f,0.f });
	~CoinEntity();

	void OnPickup(GameObjectEntity* pickupper);
};
#endif