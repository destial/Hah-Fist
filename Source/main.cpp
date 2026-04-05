/*!
* @file main.cpp
* @author Rance Andres (andresrancerowell.g@digipen.edu)
* @date 9 January 2026
* @course CSD1451
* @brief Main source file to start the application
*/

#include <crtdbg.h> // To check for memory leaks
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "Utils/Utils.hpp"
#include "Utils/MeshRenderer.hpp"
#include "Managers/SceneManager.hpp"
#include "Managers/InputManager.hpp"

#include "Managers/AssetManager.hpp"
#include "Managers/CameraManager.hpp"
#include "Events/InputEvent.hpp"
#include "Scenes/GameScene.hpp"
#include "UI/Debug.hpp"

namespace Game {
	bool game_running; // Game running state
	Color bgd_color; // Background color
	AEAudioGroup music_grp; // AEAudio music group
	AEAudioGroup sfx_grp; // AEAudio sound effects group
	float music_vol; // Music volume
	float sfx_vol; // Sound effects volume

	void SetGameRunning(bool b) {
		game_running = b;
	}

	void SetBackgroundColor(Color c) {
		bgd_color = c;
	}

	AEAudioGroup const& GetMusicGroup() {
		return music_grp;
	}

	float& MusicVol() {
		return music_vol;
	}

	AEAudioGroup const& GetSfxGroup() {
		return sfx_grp;
	}

	float& SfxVol() {
		return sfx_vol;
	}
}

// Program Entrypoint
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE,
	_In_ LPWSTR,
	_In_ int       nCmdShow) {

#if _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Using custom window procedure
	s32 console = 0;
#if _DEBUG
	console = 1;
#endif
	AESysInit(hInstance, nCmdShow, 1600, 900, console, 0, false, NULL);

	// Changing the window title & icon
	AESysSetWindowTitle("Hah Fist!");
	AESysSetWindowIcon("Assets/icon.ico", 24, 24);

	// Reset the system modules
	AESysReset();
	AEFrameRateControllerInit(static_cast<u32>(-1));

	// Initialize fonts
	AEGfxFontSystemStart();

	// Initialize game settings
	Game::game_running = true;
	DebugUtils::ToggleRender(false);

	// Initialize background color
	Game::bgd_color = true;
	Game::bgd_color = { 1.f, 0.3f, 0.3f, 0.3f };

	// Create & initialize audio groups & volume for music & sfx
	Game::music_grp = AEAudioCreateGroup();
	while (!AEAudioIsValidGroup(Game::music_grp)) {
		AEAudioUnloadAudioGroup(Game::music_grp);
		Game::music_grp = AEAudioCreateGroup();
	}
	Game::music_vol = 1.f;

	Game::sfx_grp = AEAudioCreateGroup();
	while (!AEAudioIsValidGroup(Game::sfx_grp)) {
		AEAudioUnloadAudioGroup(Game::sfx_grp);
		Game::sfx_grp = AEAudioCreateGroup();
	}
	Game::sfx_vol = 1.f;

	{
		SceneManager sceneManager;
		// Don't need to call delete after, already handled in ~scene manager destructor
		sceneManager.SetNextScene(Scenes::SPLASH_SCREEN);

		// Game Loop
		while (Game::game_running) {
			// Informing the system about the loop's start
			AESysFrameStart();
			AEFrameRateControllerStart();

			// Set the delta time
			f32 frame_time = static_cast<f32>(AEFrameRateControllerGetFrameTime());

			// F11 fullscreen toggle
			if (AEInputCheckTriggered(AEVK_F11)) {
				if (AESysIsFullScreen())
					AESysSetFullScreen(0);
				else
					AESysSetFullScreen(1);
			}

			// Set window resolution
			Utils::SetScreenResolution(AEGfxGetWindowWidth(), AEGfxGetWindowHeight());

			// -=-=-=-=-=-=-=- Update Logic Start -=-=-=-=-=-=-=-
			float dt;
			// Run timestep for every lost frame if < 60fps
			while (frame_time > 0.f) {
				dt = min(frame_time, 1 / 60.f);
				Utils::Setdelta_time(dt);
				sceneManager.PreUpdate(dt);
				InputManager::GetInstance()->Update(dt);
				sceneManager.Update(dt);
				sceneManager.PostUpdate(dt);
				frame_time -= dt;
			}
			InputManager::GetInstance()->EndFrame();

			// -=-=-=-=-=-=-=- Rendering Logic Start -=-=-=-=-=-=-=-

			// Set background color
			AEGfxSetBackgroundColor(Game::bgd_color.r / 255.f, Game::bgd_color.g / 255.f, Game::bgd_color.b / 255.f);
			sceneManager.Render();

			// Informing the system about the loop's end
			AEFrameRateControllerEnd();
			AESysFrameEnd();

			// Exit application if window is forced closed
			if (0 == AESysDoesWindowExist())
				Game::bgd_color = false;
		}

		// Clean up the current scene's resources
		sceneManager.GetCurrentScene()->End();
	}

	// Unload audio groups
	AEAudioUnloadAudioGroup(Game::music_grp);
	AEAudioUnloadAudioGroup(Game::sfx_grp);

	// Clean up other resources
	CameraManager::Free();
	MeshRenderer::Free();
	InputManager::Free();
	AssetManager::Free();
	AESysExit();
}
