#pragma once
#ifndef __CIRCLEBUTTONUI_H
#define __CIRCLEBUTTONUI_H
#include "BaseUI.hpp"
#include <functional>
#include <vector>

class CircleButtonUI : public BaseUI {
protected:
	std::vector<std::function<void(MouseButton)>> click_listeners;
	std::vector<std::function<void()>> hover_listeners;
	std::vector<std::function<void()>> unhover_listeners;
	bool mouse_hovered;
	bool clicked_this_frame;
public:
	f32 radius;
	CircleButtonUI(AEVec2 pos);
	virtual ~CircleButtonUI();

	void AddClickListener(std::function<void(MouseButton)> func);
	void AddHoverListener(std::function<void()> func);
	void AddStopHoverListener(std::function<void()> func);

	virtual void OnMouseClick(AEVec2 mousePos, MouseButton button);
	virtual void OnMouseHover(AEVec2 pos);
	virtual void OnMouseStopHover();

	virtual void PostUpdate(const f32& dt);
	virtual void Update(const f32& dt);
	virtual void Render();
};

#endif