/*!
* @file SpiderEntity.hpp
* @author Name (brandonshaohui.koh@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief This source file contains the declaration of class SpiderEntity as well as its member functions
*		 including the behaviours.
*/
#pragma once
#ifndef __SPIDERENTITY_H
#define __SPIDERENTITY_H
#include "EnemyEntity.hpp"
/*!
* @brief SpiderEntity class that handles the behaviour of the spider
*/
class SpiderEntity : public EnemyEntity {
protected:
	bool spawn_hatchlings;
public:
	/*!
	* @brief SpiderEntity constructor
	* @param pos - Position of the SpiderEntity
	* @param speed - Speed of the SpiderEntity
	* @param spawn_hatchlings - If true SpiderEntity will spawn tiny spiders
	*/
	SpiderEntity(AEVec2 pos, f32 speed = 10.f, bool spawn_hatchlings = true);

	/*!
	* @brief Destructor of SpiderEntity
	*/
	~SpiderEntity() override;

	/*!
	* @brief PostUpdate function of the SpiderEntity
	* @param dt - Time between every frame
	*/
	void PostUpdate(const f32& dt) override;

	/*!
	* @brief OnCollide function which takes care of the collision response of the SpiderEntity
	* @param go - GameObjectEntity it is colliding with
	*/
	void OnCollide(GameObjectEntity* go) override;

	// Virtual methods to Enemy Base class

	/*!
	* @brief OnHit function which takes care of the collision effects
	*/
	void OnHit() override;

	/*!
	* @brief OnIdle function which takes care of the SpiderEnttiy idle behaviour
	* @param dt - Time between every frame
	*/
	void OnIdle(const f32& dt) override;

	/*!
	* @brief OnDead function which takes care of the Dead behaviour of the SpiderEntity
	*/
	void OnDead() override;
};



#endif
