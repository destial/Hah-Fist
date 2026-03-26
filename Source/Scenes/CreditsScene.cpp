/*!
* @file CreditsScene.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 2 March 2026
* @course CSD1451
* @brief Definition file for a credits scene
*/

#include "CreditsScene.hpp"
#include "../UI/ImageUI.hpp"
#include "../Utils/Utils.hpp"
#include "../Managers/AssetManager.hpp"
#include "../Managers/SceneManager.hpp"

CreditsScene::CreditsScene() : BaseScene{} {} // Empty ctor

CreditsScene::~CreditsScene() {} // Empty dtor

/*!
* @brief Inherited: Initialize the scene with the credits scene image
*/
void CreditsScene::Init() {
	// Initialize credits as a sprite sheet
	ImageUI* screen = new ImageUI{ ASSET_CREDITS_SPRITE, { Utils::GetWorldWidth() * 0.5f, Utils::GetWorldHeight() * 0.5f }, 1, 3 };
	screen->scale = { Utils::GetWorldWidth(), Utils::GetWorldHeight() };
	screen->color.a = 0;
	screen->SetSpriteDuration(5.f);
	screen->SetInteractive(false);

	scene_entities.push_back(screen);
	Game::SetBackgroundColor({ 1.f, 0.f, 0.f, 0.f });
	timer = 0.f;
};

/*!
* @brief Inherited: Update all entities in the scene
* @param dt - The delta time for this frame
*/
void CreditsScene::Update(const f32& dt) {
	BaseScene::Update(dt);

	// Fade the background image according to time
	scene_entities[0]->color.a = static_cast<unsigned char>(timer < 1.f ? 255 * timer : (timer > 4.f ? 255 * (5.f - timer) : 255));

	if ((timer += dt) > 5.f) { // Once 5s has passed, transition to main menu
		SceneManager::GetInstance()->SetNextScene(Scenes::MAIN_MENU);
	}
}