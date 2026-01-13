#include "ButtonUI.hpp"

ButtonUI::ButtonUI(AEVec2 pos) : BaseUI(pos), listeners(0) {}

ButtonUI::ButtonUI() : BaseUI(), listeners(0) {}

ButtonUI::~ButtonUI() {
	std::printf("Called Player deconstructor\n");
	listeners.clear();
}

void ButtonUI::AddListener(std::function<void()> func) {
	listeners.push_back(func);
}

void ButtonUI::OnMouseClick(AEVec2 mousePos, MouseButton button) {
	std::printf("click!\n");
}

void ButtonUI::OnMouseHover(AEVec2 pos) {
}

void ButtonUI::OnMouseStopHover() {
}
