#include "SpriteSheet.hpp"
#include "../Utils/MeshRenderer.hpp"

#include "../Managers/AssetManager.hpp"

SpriteSheet::SpriteSheet(const char* file, int rows, int columns) : rows(rows), columns(columns) {
	texture = AEGfxTextureLoad(file);
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.f, 1.f / rows,
		0.5f, -0.5f, 0xFFFFFFFF, 1.f / columns, 1.f / rows,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f / columns, 1.f / rows,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f / columns, 0.f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f);
	mesh = AEGfxMeshEnd();
}

SpriteSheet::~SpriteSheet() {
	if (texture) {
		AEGfxTextureUnload(texture);
	}
	texture = nullptr;
	AEGfxMeshFree(mesh);
	mesh = nullptr;
}

void SpriteSheet::Render(AEMtx33& t, int row, int column) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	if (texture) {
		AEGfxTextureSet(texture, static_cast<f32>(column) / columns, static_cast<f32>(row) / rows);
	}
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(t.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
}
