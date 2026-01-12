#include "BaseEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"

BaseEntity::BaseEntity(AEVec2 position) {
	this->position = position;
	this->velocity = { 0.f, 0.f };
	this->scale = { 1.f, 1.f };
	this->rotation = 0.f;
	this->mesh = nullptr;
	this->transform = { 0 };
}

BaseEntity::~BaseEntity() {
	if (mesh) {
		AEGfxMeshFree(mesh);
		mesh = nullptr;
	}
}

void BaseEntity::PreUpdate(const f32& dt) {}
void BaseEntity::Update(const f32& dt) {}

void BaseEntity::PostUpdate(const f32& dt) {
	this->position += (this->velocity);
}

void BaseEntity::Render() {
	// TODO: Convert to world coordinates
	AEMtx33 scale = { 1.f };
	AEMtx33Scale(&scale, this->scale.x, this->scale.y);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, this->rotation);
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, this->position.x, this->position.y);
	this->transform = { 0 };
	AEMtx33Concat(&this->transform, &rotate, &scale);
	AEMtx33Concat(&this->transform, &translate, &transform);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
}