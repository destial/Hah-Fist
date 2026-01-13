#include "BaseUI.hpp"

BaseUI::BaseUI(AEVec2 pos) : BaseEntity(pos),
	overlay_texture(nullptr),
	overlay_color(0xFFFFFFFF),
	mouse_hovered(false),
	clicked_this_frame(false)
{

}

BaseUI::~BaseUI() {
	if (overlay_texture) {
		AEGfxTextureUnload(overlay_texture);
		overlay_texture = nullptr;
	}
}
