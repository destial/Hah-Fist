#pragma once
#ifndef __BASEUI_H
#define __BASEUI_H
#include <string>
#include <vector>
#include <functional>
#include "../Entities/BaseEntity.hpp"

typedef enum MouseButton {
	LEFT = 0,
	RIGHT,
	MIDDLE
} MouseButton;

class BaseUI : public BaseEntity {
protected:
	s8 font;
	std::vector<std::function<void()>> update_listeners;
public:
	std::string text;
	f32 text_size;
	u32 base_color;
	u32 overlay_color;
	AEGfxTexture* overlay_texture;

	BaseUI(AEVec2 pos);
	virtual ~BaseUI();
	virtual void Update(const f32& dt);
	virtual void Render();
	virtual void RenderText();
};

#endif