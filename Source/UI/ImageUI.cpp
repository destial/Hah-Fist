#include "ImageUI.hpp"
#include "../Managers/AssetManager.hpp"

ImageUI::ImageUI(const char* file_name, AEVec2 pos, int r, int c)
: BaseUI{ pos },
  animationFrame{0}, animationTimer{0}, currentCol{0}, currentRow{0},
  sprite{ r == 0 ? nullptr : AssetManager::GetSpriteSheet(file_name, r, c) } {
	text = "";
	image = r == 0 ? AssetManager::GetTexture(file_name) : nullptr;
}

ImageUI::~ImageUI() {}

void ImageUI::Update(const f32& dt) {
	BaseUI::Update(dt);
	if (sprite) {
		if ((animationTimer += dt) > (animationFrame / (sprite->Columns() * sprite->Rows()))) {
			animationTimer = 0.f;
			if (++currentCol > sprite->Columns()) {
				currentCol = 0;
				if (++currentRow > sprite->Rows()) {
					currentRow = 0;
				}
			}
		}
	}
}

void ImageUI::Render() {
	if (sprite) {
		sprite->Render(transform, color, currentRow, currentCol);
		return;
	}

	if (image && image->data) {
		BaseUI::Render();
		return;
	}
}

f32 ImageUI::GetSpriteDuration() const {
	return animationFrame;
}

void ImageUI::SetSpriteDuration(f32 dur) {
	animationFrame = dur;
}
