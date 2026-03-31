/*!
* @file SpikeProjectile.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 13 March 2026
* @course CSD1451
* @brief ExplosiveProjectile class that represents a projectile which triggers
*        an explosion upon impact or expiration. Extends BaseProjectile to add
*        area-of-effect damage behavior.
*/
#include "SpikeProjectile.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Managers/AssetManager.hpp"
/*!
* @brief Constructs a SpikeProjectile with position, direction, speed, damage, and owner
* @param pos - Initial position of the projectile
* @param dir - Direction vector of movement
* @param speed - Maximum movement speed of the projectile
* @param dmg - Damage dealt upon collision
* @param own - Pointer to the owning entity
* @return None
*/
SpikeProjectile::SpikeProjectile(AEVec2 pos, AEVec2 dir, f32 speed, f32 dmg, GameObjectEntity* own) 
: BaseProjectile{pos, dir, speed, dmg, own} {
    //Adjust sprite to spike and spike image is added
    sprite = AssetManager::GetSpriteSheet(ASSET_SPIKE_IMAGE, 1, 1); // single-frame bullet
    lifetime = BOSS1SPIKELIFETIME;
}
/*!
* @brief Destructor for SpikeProjectile
* @return None
*/
SpikeProjectile::~SpikeProjectile() {} // Empty dtor
