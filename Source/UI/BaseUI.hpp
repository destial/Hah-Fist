#pragma once
#ifndef __BASEUI_H
#define __BASEUI_H
#include <string>
#include "../Entities/BaseEntity.hpp"
#include "../Utils/Utils.hpp"

class BaseUI : public BaseEntity {
public:
	enum MouseButton {
		LEFT = AEVK_LBUTTON,
		RIGHT = AEVK_RBUTTON,
		MIDDLE = AEVK_MBUTTON
	};

	enum TEXT_ALIGNMENT {
		CENTER,
		LEFT_CORNER,
		RIGHT_CORNER
	} text_alignment;
	s8 font;
	std::string text;
	f32 text_size;
	Color overlay_color;
	AEGfxTexture* overlay_texture;

	BaseUI(AEVec2 pos = { 0.f, 0.f });
	virtual ~BaseUI();
	virtual void Render();
	virtual void RenderText();
};

#endif