#pragma once
#ifndef __BASEUI_H
#define __BASEUI_H
#include <string>
#include "../Entities/BaseEntity.hpp"
#include "../Utils/Utils.hpp"

typedef enum MouseButton {
	LEFT = 0,
	RIGHT,
	MIDDLE
} MouseButton;

class BaseUI : public BaseEntity {
protected:
	s8 font;
public:
	std::string text;
	f32 text_size;
	Color overlay_color;
	AEGfxTexture* overlay_texture;

	BaseUI(AEVec2 pos);
	virtual ~BaseUI();
	virtual void Render();
	virtual void RenderText();
};

#endif