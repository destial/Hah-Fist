/*!
* @file SpriteSheet.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 13 January 2026
* @course CSD1451
* @brief Definition file for sprite sheets used for animation
*/
#include "SpriteSheet.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Managers/AssetManager.hpp"

SpriteSheet::SpriteSheet(const char* file, int rows, int columns) : rows{ rows }, columns{ columns } {
	image = new Image{ file };
	AEGfxMeshStart(); // Create mesh with correct UV texture coordinates
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.f, 1.f / rows,
		0.5f, -0.5f, 0xFFFFFFFF, 1.f / columns, 1.f / rows,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f / columns, 1.f / rows,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f / columns, 0.f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.f, 0.f);
	mesh = AEGfxMeshEnd();
} // Ctor

SpriteSheet::~SpriteSheet() { // Dtor
	delete image;
	image = nullptr;
	AEGfxMeshFree(mesh);
	mesh = nullptr;
}

/*!
* @brief Get the max rows of this spritesheet
*/
int SpriteSheet::Rows() const {
	return rows;
}

/*!
* @brief Get the max columns of this spritesheet
*/
int SpriteSheet::Columns() const {
	return columns;
}

/*!
* @brief Render the spritesheet
* @param t - The transformation matrix to apply
* @param color - The color to multiply with
* @param row - The spritesheet row
* @pararm column - The spritesheet column
*/
void SpriteSheet::Render(AEMtx33& t, Color color, int row, int column) {
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	// If image data exists (file exists)
	if (image->data) {
		AEGfxTextureSet(image->data, static_cast<f32>(column) / columns, static_cast<f32>(row) / rows);
	}
	AEGfxSetColorToMultiply(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(color.a / 255.f);
	AEGfxSetTransform(t.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
}
