/*!
* @file BaseProjectile.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 3 March 2026
* @course CSD1451
* @brief Declaration of the BaseProjectile class which represents a generic
*        projectile entity in the game. This class handles movement,
*        lifetime tracking, animation, collision handling, and ownership.
*        It serves as a base class for all projectile types.
*/
#pragma once
#ifndef __BASEPROJECTILE_H
#define __BASEPROJECTILE_H

#include "../GameObjectEntity.hpp"
#include "../../Items/SpriteSheet.hpp"
/*!
* @brief BaseProjectile class that handles projectile behavior such as movement,
*        lifetime management, animation, collision response, and ownership logic
*/
class BaseProjectile : public GameObjectEntity {
protected:
    f32 lifetime{ BULLETLIFETIME };
    f32 age{ 0.f };
    AEVec2 direction;
    f32 speed;
    f32 animationTimer, animationFrame;
    int currentRow, currentCol;
    SpriteSheet* sprite;
    GameObjectEntity* owner;
public:
    /*!
    * @brief Constructs a BaseProjectile with initial position, direction, speed, damage, and owner
    * @param pos - Initial position of the projectile
    * @param dir - Direction vector of movement
    * @param speed - Movement speed of the projectile
    * @param damage - Damage dealt upon collision
    * @param _owner - Pointer to the entity that owns this projectile
    * @return None
    */
    BaseProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 damage, GameObjectEntity* _owner);
    /*!
    * @brief Virtual destructor for proper cleanup of derived projectile types
    * @return None
    */
    virtual ~BaseProjectile() = default;
    /*!
    * @brief Handles logic before the main update (e.g., input or preparation)
    * @param dt - Delta time since last frame
    * @return None
    */
    virtual void PreUpdate(const f32& dt);
    /*!
    * @brief Updates projectile state including movement, animation, and lifetime
    * @param dt - Delta time since last frame
    * @return None
    */
    virtual void Update(const f32& dt);
    /*!
    * @brief Renders the projectile using its sprite and animation state
    * @return None
    */
    virtual void Render();
    /*!
    * @brief Called when the projectile collides with another game object
    * @param other - Pointer to the object collided with
    * @return None
    */
    virtual void OnCollide(GameObjectEntity* other);
    /*!
    * @brief Handles logic when the projectile successfully hits another object
    * @param other - Pointer to the object hit
    * @return None
    */
    virtual void OnHit(GameObjectEntity* other);
    /*!
    * @brief Attempts to transfer ownership of the projectile to another entity
    * @param newOwner - Pointer to the new owner entity
    * @return True if ownership change is successful, false otherwise
    */
    bool TryChangeOwnership(GameObjectEntity* newOwner);

protected:
    /*!
    * @brief Handles logic when the projectile expires (e.g., lifetime exceeded)
    * @return None
    */
    virtual void OnExpire();                      
};

#endif