#pragma once
#ifndef __IMAGEUI_H
#define __IMAGEUI_H
#include "BaseUI.hpp"
#include "../Items/Image.hpp"
#include "../Items/SpriteSheet.hpp"

class ImageUI : public BaseUI {
protected:
	SpriteSheet* sprite; // Sprite animation data
public:
	f32 animationTimer, animationFrame; // Sprite animation data
	int currentRow, currentCol; // Sprite animation data

	ImageUI(const char* file_name, AEVec2 pos = { 0.f, 0.f }, int rows = 0, int cols = 0); // Ctor
	ImageUI(ImageUI const&) = delete; // Remove copy ctor
	ImageUI& operator=(ImageUI const&) = delete; // Remove copy assignment
	virtual ~ImageUI(); // Dtor

	virtual void Update(const f32& dt);
	virtual void Render();

	f32 GetSpriteDuration() const;
	void SetSpriteDuration(f32 dur);
};

#endif