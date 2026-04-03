#include "MovingPlatformEntity.hpp"
#include "../../Utils/AEOverload.hpp"
#include "../../Utils/MeshRenderer.hpp"
#include "../../UI/Debug.hpp"
#include "../../Managers/SceneManager.hpp"

MovingPlatformEntity::MovingPlatformEntity(AEVec2 pos, AEVec2 travelDir, bool loopMovement, f32 speed, f32 life) 
: StaticEntity{ STATIC_TYPE::TYPE_PLATFORM, pos, 1.0f, CollisionShape::AABB, PhysicsType::MOVING_STATIC },
  travel_direction{ travelDir },looping_movement{ loopMovement }, travel_speed{ speed }, lifetime{ life } {
	AEVec2 length = start_point - end_point;
	normal_distance = AEVec2Length(&length);
	mesh = MeshRenderer::GetCenterRectMesh();
	layer = BaseEntity::RenderLayer::WORLD;
	SetStartPoint(pos);
}

MovingPlatformEntity::~MovingPlatformEntity() {}// Empty dtor

void MovingPlatformEntity::Update(const f32& dt) {
	if (lifetime > 0.f) {
		lifetime -= dt;
		if (lifetime <= 0.f) {
			SceneManager::GetInstance()->GetCurrentScene()->RemoveEntityFromScene(this);
		}
	}
	prev_position = position;
	if (normal_direction) {
		position += (end_point - start_point) * travel_speed * dt;
		AEVec2 length = position - start_point;
		if (!looping_movement) { return; }
		if (normal_distance < AEVec2Length(&length)) {
			normal_direction = !normal_direction;
		}
	}
	else {
		position -= (end_point - start_point) * travel_speed * dt;
		AEVec2 length = position - start_point;
		if (normal_distance < AEVec2Length(&length)) {
			normal_direction = !normal_direction;
		}
	}
}

void MovingPlatformEntity::Render() {
	StaticEntity::Render();
	DebugUtils::RenderLine(start_point, end_point, { 64, 255, 255, 255 });
}

void MovingPlatformEntity::OnCollide(GameObjectEntity* go){
	StaticEntity::OnCollide(go);
	if (go->go_type == PhysicsType::DYNAMIC) {
		go->position.x += position.x - prev_position.x;
	}
}

void MovingPlatformEntity::SetStartPoint(AEVec2 start) {
	start_point = start;
	end_point = start + travel_direction * 10.f;
}