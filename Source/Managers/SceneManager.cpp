#include "SceneManager.hpp"

SceneManager::SceneManager() : current_scene(0), prev_scene(0) {
}

SceneManager::~SceneManager() {
}

void SceneManager::PreUpdate(const f32& dt) {
	if (prev_scene != current_scene) {
		if (prev_scene)
			prev_scene->End();

		if (current_scene)
			current_scene->Init();
	}

	prev_scene = current_scene;

	if (prev_scene)
		prev_scene->PreUpdate(dt);
}

void SceneManager::Update(const f32& dt) {
	if (prev_scene)
		prev_scene->Update(dt);
}

void SceneManager::PostUpdate(const f32& dt) {
	if (prev_scene)
		prev_scene->PostUpdate(dt);
}

void SceneManager::Render() {
	// Tell the engine to get ready to draw something with color.
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	if (prev_scene)
		prev_scene->Render();
}

void SceneManager::SetNextScene(BaseScene* next) {
	this->current_scene = next;
}

const BaseScene* SceneManager::GetCurrentScene() {
	return current_scene;
}
