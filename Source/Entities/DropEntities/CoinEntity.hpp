/*!
* @file CoinEntity.hpp
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 12th March 2026
* @course CSD1451
* @brief This source file defines the CoinEntity class and its member functions, which are
* collectible objects.
*/
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