#include "SpriteAnimation.hpp"
#include "../Utils/MeshRenderer.hpp"

#include "../Managers/AssetManager.hpp"

SpriteAnimation::SpriteAnimation(const char* file, int rows, int columns) : rows(rows), columns(columns) {
	texture = AssetManager::GetTexture(file);
	AEGfxMeshStart();
	AEGfxTriAdd(
		0.f, 0.f, 0xFFFFFFFF, 0.f, 1.f / rows,
		1.f, 0.f, 0xFFFFFFFF, 1.f / columns, 1.f / rows,
		0.f, 1.f, 0xFFFFFFFF, 0.f, 0.f);
	AEGfxTriAdd(
		1.f, 0.f, 0xFFFFFFFF, 1.0f / columns, 1.f / rows, 
		1.f, 1.f, 0xFFFFFFFF, 1.0f / columns, 0.f,
		0.f, 1.f, 0xFFFFFFFF, 0.f, 0.f);
	mesh = AEGfxMeshEnd();
}

SpriteAnimation::~SpriteAnimation() {
	texture = nullptr;
	AEGfxMeshFree(mesh);
	mesh = nullptr;
}

void SpriteAnimation::Render(AEMtx33& t, int row, int column) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(texture, static_cast<f32>(column) / columns, static_cast<f32>(row) / rows);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(t.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
}
