#pragma once
#ifndef __BASEUI_H
#define __BASEUI_H
#include <string>
#include "../Entities/BaseEntity.hpp"
#include "../Utils/Utils.hpp"
#include "../Items/Image.hpp"

class BaseUI : public BaseEntity {
protected:
	bool mouse_hovered;
	bool clicked_this_frame;
	BaseUI(AEVec2 pos = { 0.f, 0.f });
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
	Color text_color;
	Color overlay_color;
	Color overlay_text_color;
	Image* overlay_texture;

	virtual ~BaseUI();
	virtual void Render();
	virtual void RenderText();
};

#endif