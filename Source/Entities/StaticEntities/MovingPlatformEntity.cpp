#include "MovingPlatformEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/MeshRenderer.hpp"

MovingPlatformEntity::MovingPlatformEntity(AEVec2 pos, AEVec2 travelDir) : StaticEntity(STATIC_TYPE::TYPE_PLATFORM, pos, 1.0f)
{
	startPoint = pos;
	endPoint = pos + travelDir * 10.f;
	AEVec2 length = startPoint - endPoint;
	normalDistance = AEVec2Length(&length);
	mesh = MeshRenderer::GetCenterRectMesh();
	layer = BaseEntity::RenderLayer::WORLD;
}

MovingPlatformEntity::~MovingPlatformEntity()
{

}

void MovingPlatformEntity::Update(const f32& dt)
{
	prev_position = position;
	if (normalDirection)
	{
		position += (endPoint - startPoint) * travelSpeed * dt;
		AEVec2 length = position - startPoint;
		if (!loopingMovement) { return; }
		if (normalDistance < AEVec2Length(&length))
		{
			normalDirection = !normalDirection;
		}
	}
	else
	{
		position -= (endPoint - startPoint) * travelSpeed * dt;
		AEVec2 length = position - startPoint;
		if (normalDistance < AEVec2Length(&length))
		{
			normalDirection = !normalDirection;
		}
	}
}

void MovingPlatformEntity::OnCollide(GameObjectEntity* go)
{
	StaticEntity::OnCollide(go);
	if (go->go_type == PhysicsType::DYNAMIC)
	{
		go->position.x += position.x - prev_position.x;
	}
}