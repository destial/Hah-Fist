#pragma once
#ifndef __BUTTONUI_H
#define __BUTTONUI_H
#include "BaseUI.hpp"
#include <functional>
#include <vector>

class ButtonUI : public BaseUI {
protected:
	std::vector<std::function<void()>> click_listeners;
	std::vector<std::function<void()>> hover_listeners;
	std::vector<std::function<void()>> unhover_listeners;
public:
	ButtonUI();
	ButtonUI(AEVec2 pos);
	virtual ~ButtonUI();

	void AddClickListener(std::function<void()>& func);
	void AddHoverListener(std::function<void()>& func);
	void AddStopHoverListener(std::function<void()>& func);
	virtual void OnMouseClick(AEVec2 mousePos, MouseButton button);
	virtual void OnMouseHover(AEVec2 pos);
	virtual void OnMouseStopHover();
};

#endif