#include "BaseUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/AssetManager.hpp"
#include <iostream>

BaseUI::BaseUI(AEVec2 pos) : BaseEntity(pos),
	overlay_texture(nullptr),
	overlay_color({255, 128, 128, 128}),
	font(0),
	text("BaseUI"),
	text_size(1.f),
	text_alignment(CENTER)
{
	layer = 5;
	mesh = MeshRenderer::GetCenterRectMesh();
	font = AssetManager::GetFontId("Assets/buggy-font.ttf");
}

BaseUI::~BaseUI() {
	std::printf("Called BaseUI deconstructor\n");
}

void BaseUI::Render() {
	if (texture) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(texture, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	AEGfxSetColorToMultiply(color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
	RenderText();
}

void BaseUI::RenderText() {
	f32 w, h;
	const char* str = text.c_str();
	AEGfxGetPrintSize(font, str, text_size, &w, &h);
	
	if (text_alignment == TEXT_ALIGNMENT::CENTER) {
		AEVec2 screen = Utils::Game_To_TextScreen(this->position.x, this->position.y);
		AEGfxPrint(font, str, screen.x - ((w / 2.f) / text_size * 0.5f), screen.y - ((h / 2.f) / text_size * 0.5f), h, 0.f, 0.f, 0.f, 1.f);
	}
	else if (text_alignment == TEXT_ALIGNMENT::LEFT_CORNER) {
		AEVec2 screen = Utils::Game_To_TextScreen(this->position.x - (this->scale.x * 0.5f), this->position.y);
		AEGfxPrint(font, str, screen.x, screen.y - ((h / 2.f) / text_size * 0.5f), h, 0.f, 0.f, 0.f, 1.f);
	}
	else if (text_alignment == TEXT_ALIGNMENT::RIGHT_CORNER) {
		AEVec2 screen = Utils::Game_To_TextScreen(this->position.x + (this->scale.x * 0.5f), this->position.y);
		AEGfxPrint(font, str, screen.x - ((w / 2.f) / text_size), screen.y - ((h / 2.f) / text_size * 0.5f), h, 0.f, 0.f, 0.f, 1.f);
	}
}
