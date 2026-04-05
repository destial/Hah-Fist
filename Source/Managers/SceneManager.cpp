#include "SceneManager.hpp"
#include "../Scenes/GameScene.hpp"
#include "../Scenes/SplashScene.hpp"
#include "../Scenes/StartMenuScene.hpp"
#include "../Scenes/CreditsScene.hpp"
#include "../UI/Debug.hpp"

namespace DebugUtils {
	void _RenderAll();
}

SceneManager::SceneManager() : all_scenes{}, next_scene{ Scenes::INIT }, current_scene{ Scenes::INIT }, editor{nullptr} {
	instance = this;
	all_scenes[Scenes::SPLASH_SCREEN] = new SplashScene();
	all_scenes[Scenes::MAIN_MENU] = new StartMenuScene();
	all_scenes[Scenes::GAME] = new GameScene();
	all_scenes[Scenes::CREDITS] = new CreditsScene();
	editor = new LevelEditor{ all_scenes[0] };
}

SceneManager::~SceneManager() {
	for (int i = 0; i < Scenes::LAST; ++i) {
		if (all_scenes[i]) {
			delete all_scenes[i];
		}
	}
	delete editor;
	instance = nullptr;
}

void SceneManager::PreUpdate(const f32& dt) {
	if (current_scene != next_scene) {
		if (current_scene != Scenes::INIT) {
			if (all_scenes[current_scene])
				all_scenes[current_scene]->End();
		}

		if (next_scene == Scenes::INIT) {
			return;
		}
		if (all_scenes[next_scene]) {
			AEAudioStopGroup(Game::GetMusicGroup());
			AEAudioStopGroup(Game::GetSfxGroup());

			all_scenes[next_scene]->Init();
			editor->SetScene(all_scenes[next_scene]);
		}
	}

	current_scene = next_scene;

	if (all_scenes[current_scene] && !editor->IsToggled()) {
		all_scenes[current_scene]->PreUpdate(dt);
	}
}

void SceneManager::Update(const f32& dt) {
	if (AEInputCheckTriggered(AEVK_P)) {
		editor->Toggle();
	}

	if (AEInputCheckTriggered(AEVK_G)) {
		DebugUtils::ToggleRender(!DebugUtils::IsRendering());
	}

	if (all_scenes[current_scene]) {
		if (!editor->IsToggled()) {
			all_scenes[current_scene]->Update(dt);
		}
		else {
			editor->Update(dt);
		}
	}
}

void SceneManager::PostUpdate(const f32& dt) {
	if (all_scenes[current_scene] && !editor->IsToggled())
		all_scenes[current_scene]->PostUpdate(dt);
}

void SceneManager::Render() {
	if (all_scenes[current_scene]) {
		all_scenes[current_scene]->Render();
		if (editor->IsToggled()) {
			editor->Render();
		}
	}

	DebugUtils::_RenderAll();

	if (DebugUtils::IsRendering()) {
		static float fps_counter = 0.f;
		static float cfps = 1.f / Utils::Getdelta_time();
		static float saved = 0.f;
		if ((fps_counter += Utils::Getdelta_time()) > 0.1f) {
			cfps = 1.f / Utils::Getdelta_time();
			fps_counter = 0.f;
		}
		char fps[50];
		sprintf_s(fps, "FPS:%.0f", cfps);
		DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 0.25f }, std::string{ fps });

		f32 cam_x, cam_y;
		AEGfxGetCamPosition(&cam_x, &cam_y);
		char cam[128];
		sprintf_s(cam, "Cam:%.2f,%.2f", cam_x, cam_y);
		DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 0.75f }, std::string{ cam });

		AEVec2 mwp = Utils::GetMouseWorld(true);
		char mos[128];
		sprintf_s(mos, "Mouse:%.2f,%.2f", mwp.x, mwp.y);
		DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 1.25f }, std::string{ mos });

		s32 scroll;
		AEInputMouseWheelDelta(&scroll);
		char scr[32];
		sprintf_s(scr, "Scroll:%d", scroll);
		DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 1.75f }, std::string{ scr });

		if (all_scenes[current_scene]) {
			char amt[32];
			sprintf_s(amt, "Entities:%d", static_cast<int>(all_scenes[current_scene]->Entities().size()));
			DebugUtils::RenderText({ 0, Utils::GetWorldHeight() - 2.25f }, std::string{ amt });
		}
	}
}

void SceneManager::SetNextScene(Scenes::SceneState next) {
	this->next_scene = next;
}

BaseScene* SceneManager::GetCurrentScene() const {
	return all_scenes[current_scene];
}

Scenes::SceneState SceneManager::GetCurrentState() const {
	return current_scene;
}

LevelEditor* SceneManager::GetEditor() const {
	return editor;
}
