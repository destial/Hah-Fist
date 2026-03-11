#include "CreditsScene.hpp"
#include "../UI/ImageUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SceneManager.hpp"

CreditsScene::CreditsScene() : BaseScene{} {}

CreditsScene::~CreditsScene() {}

void CreditsScene::Init() {
	ImageUI* screen = new ImageUI{ ASSET_CREDITS_SPRITE, { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, 1, 3 };
	screen->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	screen->color.a = 0;
	screen->SetSpriteDuration(5.f);
	screen->SetInteractive(false);

	scene_entities.push_back(screen);
	Game::SetBackgroundColor(Color{ 1.f, 0.f, 0.f, 0.f });
};

void CreditsScene::Update(const f32& dt) {
	BaseScene::Update(dt);
	static f32 timer = 0.f;
	scene_entities[0]->color.a = static_cast<unsigned char>(timer < 1.f ? 255 * timer : (timer > 4.f ? 255 * (5.f - timer) : 255));
	if ((timer += dt) > 5.f) {
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}