#include "BaseEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"

BaseEntity::BaseEntity(AEVec2 position) {
	this->position = position;
	this->velocity = { 0.f, 0.f };
	this->scale = { 1.f, 1.f };
	this->rotation = 0.f;
	this->mesh = nullptr;
	this->texture = nullptr;
	this->transform = { 0 };
}

BaseEntity::~BaseEntity() {
	if (mesh) {
		AEGfxMeshFree(mesh);
		mesh = nullptr;
	}
	if (texture) {
		AEGfxTextureUnload(texture);
		texture = nullptr;
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

	if (texture) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(texture, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
}