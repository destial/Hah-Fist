#include "SplashScreen.hpp"
#include "../UI/ImageUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SceneManager.hpp"

SplashScreen::SplashScreen() : BaseScene{} {}

SplashScreen::~SplashScreen() {}

void SplashScreen::Init() {
	ImageUI* screen = new ImageUI{ ASSET_SPLASHSCREEN_IMAGE, { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f } };
	screen->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	screen->color.a = 0;
	screen->SetInteractive(false);

	scene_entities.push_back(screen);
	Game::SetBackgroundColor(Color{ 1.f, 0.f, 0.f, 0.f });
	timer = 0.f;
};

void SplashScreen::Update(const f32& dt) {
	BaseScene::Update(dt);
	scene_entities[0]->color.a = static_cast<unsigned char>(timer < 1.f ? 255 * timer : (timer > 1.5f ? 255 * (2.5f - timer) : 255));
	if ((timer += dt) > 2.5f) {
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}