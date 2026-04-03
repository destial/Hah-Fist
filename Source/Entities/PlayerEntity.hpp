/*!
* @file PlayerEntity.hpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @author Brandon Koh (brandonshaohui.koh@digipen.edu)
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @author Ryan Lau (r.lau@digipen.edu)
* @date 9 January 2026
* @course CSD1451
* @brief Declaration file for the player entity
*/

#pragma once
#ifndef __PLAYERENTITY_H
#define __PLAYERENTITY_H
#include "GameObjectEntity.hpp"
#include "../Items/SpriteSheet.hpp"
#include "../Events/InputEvent.hpp"
#include "../Entities/WeaponEntity.hpp"
#include <vector>

class PlayerEntity : public GameObjectEntity {
protected:
	f32 animation_timer, animation_frame; // Sprite animation data
	int current_row, current_col; // Sprite animation data
	SpriteSheet* sprite; // Sprite animation data
	f32 jump_height;
	f32 jump_velocity;
	f32 speed;
	std::vector<WeaponEntity*> weapons;
	int coin_count{ 0 };
public:
	PlayerEntity(AEVec2 pos = { 0.f, 0.f }); // Ctor
	PlayerEntity(PlayerEntity const&) = delete; // Remove copy ctor
	PlayerEntity& operator=(PlayerEntity const&) = delete; // Remove copy assignment
	virtual ~PlayerEntity(); // Dtor

	/*!
	* @brief Inherited: Pre-update the entity, before any input has been processed
	*/
	virtual void PreUpdate(const f32& dt);

	/*!
	* @brief Inherited: Update the entity after input has been processed
	*/
	virtual void Update(const f32& dt);

	/*!
	* @brief Inherited: Post-update the entity after everything else has been processed
	*/
	virtual void PostUpdate(const f32& dt);

	/*!
	* @brief Inherited: Render the entity to the screen
	*/
	virtual void Render();

	/*!
	* @brief Inherited: Called when player collides with another GameObject entity
	* @param go - The collided GameObject entity
	*/
	virtual void OnCollide(GameObjectEntity* go);

	/*!
	* @brief Add a weapon to the player
	* @param weapon - The weapon to add
	*/
	void AddWeapon(WeaponEntity* weapon);

	/*!
	* @brief Switch the current weapon of the player
	* @param index - The weapon index
	*/
	void SwitchWeapon(int index);

	/*!
	* @brief Get the current weapon of the player
	* @return The current active weapon
	*/
	WeaponEntity* CurrentWeapon() const;

	/*!
	* @brief Adds a coin to the player
	*/
	void AddCoin();

	/*!
	* @brief Get the collected coins
	* @return How many coins have been collected
	*/
	int Coins() const;

	/*!
	* @brief Get the speed of the player
	* @return The float speed value
	*/
	f32 GetSpeed() const;

	/*!
	* @brief Make the player jump
	*/
	void Jump();

	/*!
	* @brief Make the player move according to direction
	* @param dir - The direction to move the player
	*/
	void Move(AEVec2 const& dir);
};

#endif