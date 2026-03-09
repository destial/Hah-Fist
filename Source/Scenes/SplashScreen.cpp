#include "SplashScreen.hpp"
#include "../UI/ButtonUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SceneManager.hpp"

SplashScreen::SplashScreen() : BaseScene{} {}

SplashScreen::~SplashScreen() {}

void SplashScreen::Init() {
	ButtonUI* screen = new ButtonUI{ { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f } };
	screen->image = AssetManager::GetTexture("Assets/splash_screen.png");
	screen->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	screen->color.a = 0;
	screen->SetInteractive(false);

	scene_entities.push_back(screen);
	Game::SetBackgroundColor(Color{ 1.f, 0.f, 0.f, 0.f });
};

void SplashScreen::Update(const f32& dt) {
	BaseScene::Update(dt);
	static f32 timer = 0.f;
	scene_entities[0]->color.a = static_cast<unsigned char>(timer < 1.f ? 255 * timer : 255 * (2.f - timer));
	if ((timer += dt) > 2.f) {
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}