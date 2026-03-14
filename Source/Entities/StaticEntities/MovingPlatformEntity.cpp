#include "MovingPlatformEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../UI/Debug.hpp"

MovingPlatformEntity::MovingPlatformEntity(AEVec2 pos, AEVec2 travelDir) : StaticEntity(STATIC_TYPE::TYPE_PLATFORM, pos, 1.0f, CollisionShape::AABB, PhysicsType::MOVING_STATIC)
{
	SetStartPoint(pos);
	travelDirection = travelDir;
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

void MovingPlatformEntity::Render()
{
	StaticEntity::Render();
	DebugUtils::RenderLine(startPoint, endPoint, { 64, 255, 255, 255 });
}

void MovingPlatformEntity::OnCollide(GameObjectEntity* go)
{
	StaticEntity::OnCollide(go);
	if (go->go_type == PhysicsType::DYNAMIC)
	{
		go->position.x += position.x - prev_position.x;
	}
}

void MovingPlatformEntity::SetStartPoint(AEVec2 _startPoint)
{
	startPoint = _startPoint;
	endPoint = _startPoint + travelDirection * 10.f;
}