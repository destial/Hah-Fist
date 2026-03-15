#ifndef __MOVINGPLATFORMENTITY_HPP
#define __MOVINGPLATFORMENTITY_HPP
#include "../StaticEntity.hpp"

class MovingPlatformEntity : public StaticEntity
{
protected:
	AEVec2 startPoint, endPoint, travelDirection;
	bool normalDirection{ true };
	bool loopingMovement;
	f32 normalDistance;
	f32 travelSpeed;
	f32 lifetime;
public:
	MovingPlatformEntity(AEVec2 pos, AEVec2 travelDir = AEVec2{0.f, 1.f}, bool loopMovement = true , f32 speed = 1.0f, f32 _lifetime = -1.f);
	~MovingPlatformEntity();

	void Update(const f32& dt) override;
	void OnCollide(GameObjectEntity* go) override;
	void Render() override;

	void SetStartPoint(AEVec2 _startPoint);
};

#endif