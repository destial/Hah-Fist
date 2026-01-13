#pragma once
#ifndef __BASEUI_H
#define __BASEUI_H
#include "../Entities/BaseEntity.hpp"

class BaseUI : BaseEntity {
protected:
	bool mouse_hovered;
	bool clicked_this_frame;
	u8 overlay_color;
	AEGfxTexture* overlay_texture;
public:
	BaseUI(AEVec2 pos);
	virtual ~BaseUI();
	virtual void Update(const f32& dt);
	virtual void Render();
};

#endif