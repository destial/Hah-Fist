#include "BaseUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Utils/MeshRenderer.hpp"
#include "../Utils/AEOverload.hpp"
#include "../Managers/AssetManager.hpp"
#include <iostream>

BaseUI::BaseUI(AEVec2 pos) : BaseEntity(pos),
	overlay_texture(nullptr),
	overlay_color(0xFFDDDDDD),
	base_color(0xFFFFFFFF),
	font(0),
	text("BaseUI"),
	text_size(1.f)
{
	layer = 5;
	mesh = MeshRenderer::GetLeftBottomCornerRect();
	font = AssetManager::GetFontId("Assets/buggy-font.ttf");
}

BaseUI::~BaseUI() {
	std::printf("Called BaseUI deconstructor\n");
	update_listeners.clear();
}

void BaseUI::Update(const f32& dt) {
	BaseEntity::Update(dt);
	for (auto& func : update_listeners) {
		func();
	}
}

void BaseUI::Render() {
	if (texture) {
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(texture, 0.f, 0.f);
	}
	else {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	}
	Color c = Utils::ConvertFromColor(base_color);
	AEGfxSetColorToMultiply(c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(this->transform.m);
	AEGfxMeshDraw(mesh, MeshRenderer::RenderMode);
	AEGfxTextureSet(nullptr, 0.f, 0.f);
	RenderText();
}

void BaseUI::RenderText() {
	AEVec2 screen = Utils::Game_To_TextScreen(this->position.x, this->position.y);
	f32 w, h;
	const char* str = text.c_str();
	AEGfxGetPrintSize(font, str, text_size, &w, &h);
	AEGfxPrint(font, str, screen.x, screen.y, h, 0.f, 0.f, 0.f, 1.f);
}

void BaseUI::AddUpdateListener(std::function<void()> func) {
	update_listeners.push_back(func);
}
