#pragma once
#ifndef __IRONSIDEENTITY_H
#define __IRONSIDEENTITY_H
#include "BossEntity.hpp"
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
	
public:
	IronsideEntity(AEVec2 pos);
	~IronsideEntity() override;
	void PostUpdate(const f32& dt) override;
	//Helper Functions
	void StunTimerBasedOnHealth();
	LANE GetRandomSpawnLane(LANE bossLane);
	// Virtual methods to Enemy Base class
	void OnIdle(const f32& dt) override;
	void OnPatrol(const f32& dt) override;
	void OnChase(const f32& dt) override;
	void OnStun(const f32& dt) override;
	void OnDead() override;


};



#endif