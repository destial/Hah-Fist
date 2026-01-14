#pragma once
#ifndef __BASEUI_H
#define __BASEUI_H
#include <string>
#include "../Entities/BaseEntity.hpp"

typedef enum MouseButton {
	LEFT = 0,
	RIGHT,
	MIDDLE
} MouseButton;

class BaseUI : public BaseEntity {
protected:
	bool mouse_hovered;
	bool clicked_this_frame;
	s8 font;
public:
	std::string text;
	u32 overlay_color;
	AEGfxTexture* overlay_texture;

	BaseUI();
	BaseUI(AEVec2 pos);
	virtual ~BaseUI();
	virtual void Update(const f32& dt);
	virtual void Render();
	virtual void RenderText();

	virtual void OnMouseHover(AEVec2 mousePos) = 0;
	virtual void OnMouseStopHover() = 0;
	virtual void OnMouseClick(AEVec2 mousePos, MouseButton button) = 0;
};

#endif