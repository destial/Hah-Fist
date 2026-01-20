#include "BaseEntity.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Utils/Utils.hpp"
#include <cstdio>

BaseEntity::BaseEntity(AEVec2 position) :
	position(position),
	velocity({ 0 }),
	scale({ 1.f, 1.f }),
	rotation(0),
	transform({ 0 }),
	layer(0),
	mesh(nullptr), texture(nullptr)
{
}

BaseEntity::~BaseEntity() {
	std::printf("Called BaseEntity deconstructor\n");
	mesh = nullptr;
	texture = nullptr;
}

void BaseEntity::PreUpdate(const f32& dt) {}

void BaseEntity::Update(const f32& dt) {}

void BaseEntity::PostUpdate(const f32& dt) {
	this->position += this->velocity * dt;
	this->position.y = AEClamp(this->position.y, 0.f, Utils::GetWorldHeight() - this->scale.y);
	if (this->position.x <= 0.f) {
		velocity.x = 0;
	}
	else if (this->position.y <= 0.f) {
		velocity.y = 0;
	}
	
	AEMtx33 scale = { 1.f };
	AEVec2 scaleWorld = Utils::Scale_To_Screen(this->scale.x, this->scale.y);
	AEMtx33Scale(&scale, scaleWorld.x, scaleWorld.y);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, this->rotation);
	AEMtx33 translate = { 0 };
	AEVec2 screenPos = Utils::Game_To_Screen(this->position.x, this->position.y);
	AEMtx33Trans(&translate, screenPos.x, screenPos.y);
	this->transform = { 0 };
	AEMtx33Concat(&this->transform, &rotate, &scale);
	AEMtx33Concat(&this->transform, &translate, &transform);
}

void BaseEntity::Render() {
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