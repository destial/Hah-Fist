/*!
* @file MovingPlatformEntity.hpp
* @author Mohammad Hafiz (mohammadhafiz.b@digipen.edu)
* @date 15th March 2026
* @course CSD1451
* @brief This source file declares the MovingPlatformEntity class and its member functions, which are
* Moving Platforms in game.
*/
#ifndef __MOVINGPLATFORMENTITY_HPP
#define __MOVINGPLATFORMENTITY_HPP
#include "../StaticEntity.hpp"

class MovingPlatformEntity : public StaticEntity {
protected:
	AEVec2 start_point, end_point, travel_direction;
	bool normal_direction{ true };
	bool looping_movement;
	f32 normal_distance;
	f32 travel_speed;
	f32 lifetime;
public:
	MovingPlatformEntity(AEVec2 pos, AEVec2 travelDir = AEVec2{0.f, 1.f}, bool loopMovement = true , f32 speed = 1.0f, f32 life = -1.f);
	virtual ~MovingPlatformEntity();

	virtual void Update(const f32& dt) override;
	virtual void OnCollide(GameObjectEntity* go) override;
	virtual void Render() override;

	void SetStartPoint(AEVec2 _startPoint);
};

#endif