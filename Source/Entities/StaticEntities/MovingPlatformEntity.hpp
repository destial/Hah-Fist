#ifndef __MOVINGPLATFORMENTITY_HPP
#define __MOVINGPLATFORMENTITY_HPP
#include "../StaticEntity.hpp"

class MovingPlatformEntity : public StaticEntity
{
protected:
	AEVec2 startPoint, endPoint, travelDirection;
	bool normalDirection{ true };
	bool loopingMovement{ true };
	f32 normalDistance;
	f32 travelSpeed{ 1.0f };
public:
	MovingPlatformEntity(AEVec2 pos, AEVec2 travelDir = AEVec2{0.f, 1.f});
	~MovingPlatformEntity();

	void Update(const f32& dt) override;
	void OnCollide(GameObjectEntity* go) override;
	void Render() override;

	void SetStartPoint(AEVec2 _startPoint);
};

#endif