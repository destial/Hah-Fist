/*!
* @file SplashScene.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Definition file for a splash screen scene
*/

#include "SplashScene.hpp"
#include "../UI/ImageUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SceneManager.hpp"

SplashScene::SplashScene() : BaseScene{} {} // Empty ctor

SplashScene::~SplashScene() {} // Empty dtor

/*!
* @brief Inherited: Initialize the scene with the splash screen image
*/
void SplashScene::Init() {
	// Initialize the background splash screen image
	ImageUI* screen = new ImageUI{ ASSET_SPLASHSCREEN_IMAGE, { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f } };
	screen->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	screen->color.a = 0;
	screen->SetInteractive(false);

	scene_entities.push_back(screen);
	Game::SetBackgroundColor({ 1.f, 0.f, 0.f, 0.f });
	timer = 0.f;
};

/*!
* @brief Inherited: Update all entities in the scene
* @param dt - The delta time for this frame
*/
void SplashScene::Update(const f32& dt) {
	BaseScene::Update(dt);

	// Fade the background image according to time
	scene_entities[0]->color.a = static_cast<unsigned char>(timer < 1.f ? 255 * timer : (timer > 1.5f ? 255 * (2.5f - timer) : 255));

	if ((timer += dt) > 2.5f) { // Once 2.5 has passed, transition to main menu
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}