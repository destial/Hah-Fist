#include <crtdbg.h> // To check for memory leaks
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "Utils/Utils.hpp"
#include "Utils/MeshRenderer.hpp"
#include "Managers/SceneManager.hpp"
#include "Managers/InputManager.hpp"

#include "Managers/AssetManager.hpp"
#include "Events/InputEvent.hpp"
#include "Scenes/MainMenuScene.hpp"

namespace Game {
	bool bGameRunning = 1;
}

// Program Entrypoint
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Hah Fist!");

	// Reset the system modules
	AESysReset();
	AEFrameRateControllerInit(-1);

	// Initialize fonts
	AEGfxFontSystemStart();

	{
#if _DEBUG
		auto OnGameExit = ([](const InputEvent* ev) {
			if (ev->IsKeyTriggered(AEVK_ESCAPE)) {
				Game::bGameRunning = false;
			}
		});
		InputEvent::Listeners += OnGameExit;
#endif

		// dont need to call delete after, already handled in ~scene manager destructor
		SceneManager sceneManager;
		sceneManager.SetNextScene(Scenes::MAIN_MENU);
		// Game Loop
		while (Game::bGameRunning) {
			// Informing the system about the loop's start
			AESysFrameStart();
			AEFrameRateControllerStart();

			// Set the delta time
			f32 frame_time = static_cast<f32>(AEFrameRateControllerGetFrameTime());

			// -=-=-=-=-=-=-=- Update Logic Start -=-=-=-=-=-=-=-
			float dt;
			// run timestep for every lost frame if < 60fps
			while (frame_time > 0.f) {
				dt = min(frame_time, 1 / 60.f);
				Utils::SetDeltaTime(dt);
				sceneManager.PreUpdate(dt);
				InputHandler::GetInstance()->Update(dt);
				sceneManager.Update(dt);
				sceneManager.PostUpdate(dt);
				frame_time -= dt;
			}
			InputHandler::GetInstance()->EndFrame();

			// -=-=-=-=-=-=-=- Rendering Logic Start -=-=-=-=-=-=-=-

			// Set background color to gray	
			AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);
			sceneManager.Render();

			// Informing the system about the loop's end
			AEFrameRateControllerEnd();
			AESysFrameEnd();

			// Basic way to trigger exiting the application
			// when ESCAPE is hit or when the window is closed
			if (0 == AESysDoesWindowExist())
				Game::bGameRunning = false;
		}

		sceneManager.GetCurrentScene()->End();

		MeshRenderer::Free();
		InputHandler::Free();
		AssetManager::Free();
	}
	AESysExit();
}
