#include "ButtonUI.hpp"

ButtonUI::ButtonUI(AEVec2 pos) : BaseUI(pos),
	click_listeners(0),
	hover_listeners(0),
	unhover_listeners(0)
{
	text = "Button";
}

ButtonUI::ButtonUI() : BaseUI(),
	click_listeners(0),
	hover_listeners(0),
	unhover_listeners(0)
{
	text = "Button";
}

ButtonUI::~ButtonUI() {
	std::printf("Called ButtonUI deconstructor\n");
	click_listeners.clear();
}

void ButtonUI::AddClickListener(std::function<void()>& func) {
	click_listeners.push_back(func);
}

void ButtonUI::AddHoverListener(std::function<void()>& func) {
	hover_listeners.push_back(func);
}

void ButtonUI::AddStopHoverListener(std::function<void()>& func) {
	unhover_listeners.push_back(func);
}

void ButtonUI::OnMouseClick(AEVec2 mousePos, MouseButton button) {
	for (auto& func : click_listeners) {
		func();
	}
}

void ButtonUI::OnMouseHover(AEVec2 pos) {
	for (auto& func : hover_listeners) {
		func();
	}
}

void ButtonUI::OnMouseStopHover() {
	for (auto& func : unhover_listeners) {
		func();
	}
}
