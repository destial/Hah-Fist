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
	bool bGameRunning;
	Color bgdColor;
	AEAudioGroup music;
	AEAudioGroup sfx;
	void SetGameRunning(bool b) {
		bGameRunning = b;
	}

	void SetBackgroundColor(Color c) {
		bgdColor = c;
	}

	AEAudioGroup const& GetMusicGroup() {
		return music;
	}

	AEAudioGroup const& GetSfxGroup() {
		return sfx;
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

	DebugUtils::ToggleRender(false);

	Game::bGameRunning = true;
	Game::bgdColor = { 1.f, 0.3f, 0.3f, 0.3f };

	Game::music = AEAudioCreateGroup();
	while (!AEAudioIsValidGroup(Game::music)) {
		AEAudioUnloadAudioGroup(Game::music);
		Game::music = AEAudioCreateGroup();
	}
	Game::sfx = AEAudioCreateGroup();
	while (!AEAudioIsValidGroup(Game::sfx)) {
		AEAudioUnloadAudioGroup(Game::sfx);
		Game::sfx = AEAudioCreateGroup();
	}
	{
		SceneManager sceneManager;
		// dont need to call delete after, already handled in ~scene manager destructor
		sceneManager.SetNextScene(Scenes::SPLASH_SCREEN);

		// Game Loop
		while (Game::bGameRunning) {
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
			// run timestep for every lost frame if < 60fps
			while (frame_time > 0.f) {
				dt = min(frame_time, 1 / 60.f);
				Utils::SetDeltaTime(dt);
				sceneManager.PreUpdate(dt);
				InputManager::GetInstance()->Update(dt);
				sceneManager.Update(dt);
				sceneManager.PostUpdate(dt);
				frame_time -= dt;
			}
			InputManager::GetInstance()->EndFrame();

			// -=-=-=-=-=-=-=- Rendering Logic Start -=-=-=-=-=-=-=-

			// Set background color
			AEGfxSetBackgroundColor(Game::bgdColor.r / 255.f, Game::bgdColor.g / 255.f, Game::bgdColor.b / 255.f);
			sceneManager.Render();

			// Informing the system about the loop's end
			AEFrameRateControllerEnd();
			AESysFrameEnd();

			// Basic way to trigger exiting the application
			// when the window is closed
			if (0 == AESysDoesWindowExist())
				Game::bGameRunning = false;
		}

		// clean up the current scene's resources
		sceneManager.GetCurrentScene()->End();
	}

	// unload audio groups
	AEAudioUnloadAudioGroup(Game::music);
	AEAudioUnloadAudioGroup(Game::sfx);

	// clean up other resources
	CameraManager::Free();
	MeshRenderer::Free();
	InputManager::Free();
	AssetManager::Free();
	AESysExit();
}
