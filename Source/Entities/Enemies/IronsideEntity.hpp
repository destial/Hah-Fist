/*!
* @file IronsideEntity.hpp
* @author Ryan Lau (r.lau@digipen.edu)
* @date 8 March 2026
* @course CSD1451
* @brief Implementation of the IronsideEntity boss class. This class defines
*        behavior for a boss enemy that operates using a lane-based system,
*        spawning platforms, firing projectiles, and executing laser attacks.
*        The boss uses an internal state machine and scales behavior based on health.
*/
#pragma once
#ifndef __IRONSIDEENTITY_H
#define __IRONSIDEENTITY_H
#include "BossEntity.hpp"
/*!
* @brief IronsideEntity class that represents a boss enemy using a lane-based
*        combat system. The boss moves between predefined lanes, spawns moving
*        platforms, fires projectiles toward the player, and executes laser attacks.
*        Behavior dynamically scales with health to increase difficulty.
*/
class IronsideEntity : public BossEntity {
public:
	enum struct INNERFSM {
		MOVE,MOVING,SPAWNPLATFORM1, SPAWNPLATFORM2, SHOOTPROJECTILE, LASER, TOTAL
	}; 
	enum LANE {
		LANE1 = 0, LANE2 = 1, LANE3 = 2, TOTAL
	};
protected:
	INNERFSM inner_state;
	LANE curr_lane;
	int next_lane_to_spawn;
	LANE lane_to_go_to;     
	float dir_to_go;
	float target_y;
	float lane_y[BOSS3TOTALLANES]{BOSS3LANE1, BOSS3LANE2, BOSS3LANE3};
	
public:
	/*!
	* @brief Constructs the IronsideEntity and initializes its movement,
	*        lane system, and internal state machine
	* @param pos - Initial position of the Ironside boss
	* @return None
	*/
	IronsideEntity(AEVec2 pos);
	/*!
	* @brief Destructor for IronsideEntity
	* @return None
	*/
	~IronsideEntity() override;
	/*!
	* @brief Handles post-update logic using base GameObjectEntity animation handling
	* @param dt - Delta time since last frame
	* @return None
	*/
	void PostUpdate(const f32& dt) override;
	//Helper Functions
	/*!
	* @brief Calculates stun duration based on current health and applies stun state
	* @return None
	*/
	void StunTimerBasedOnHealth();
	/*!
	* @brief Selects a random lane different from the current boss lane
	* @param bossLane - Current lane occupied by the boss
	* @return A randomly selected lane that is not the current lane
	*/
	LANE GetRandomSpawnLane(LANE bossLane);
	/*!
	* @brief Handles idle state behavior and transitions to chase when activated
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnIdle(const f32& dt) override;
	/*!
	* @brief Handles patrol state behavior (currently unused)
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnPatrol(const f32& dt) override;
	/*!
	* @brief Handles chase behavior using an internal FSM for movement, platform spawning,
	*        projectile attacks, and laser execution
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnChase(const f32& dt) override;
	/*!
	* @brief Handles stun state behavior and transitions back to chase when timer expires
	* @param dt - Delta time since last frame (unused)
	* @return None
	*/
	void OnStun(const f32& dt) override;
	/*!
	* @brief Handles boss death by triggering win condition and removing the entity
	* @return None
	*/
	void OnDead() override;


};



#endif