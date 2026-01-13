#pragma once
#ifndef __BUTTONUI_H
#define __BUTTONUI_H
#include "BaseUI.hpp"
#include <functional>
#include <vector>

class ButtonUI : public BaseUI {
protected:
	std::vector<std::function<void()>> listeners;
public:
	ButtonUI();
	ButtonUI(AEVec2 pos);
	virtual ~ButtonUI();

	void AddListener(std::function<void()> func);
	virtual void OnMouseClick(AEVec2 mousePos, MouseButton button);
	virtual void OnMouseHover(AEVec2 pos);
	virtual void OnMouseStopHover();
};

#endif