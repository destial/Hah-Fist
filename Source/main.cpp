#include <crtdbg.h> // To check for memory leaks
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "Managers/SceneManager.hpp"
#include "Managers/InputManager.hpp"
#include "Events/InputEvent.hpp"
#include "Scenes/MainMenuScene.hpp"

bool bGameRunning = 1;

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
	AEFrameRateControllerInit(120);

	// Initialize fonts
	AEGfxFontSystemStart();

	{
		auto OnGameExit = [](const InputEvent* ev) {
			for (auto key : ev->GetKeysTriggered()) {
				if (key == AEVK_ESCAPE) {
					bGameRunning = false;
				}
			}
		};
		InputEvent::Listeners.push_back(OnGameExit);

		SceneManager sceneManager;
		BaseScene* scenes[1] = { new MainMenuScene() };
		sceneManager.SetNextScene(scenes[0]);
		// Game Loop
		while (bGameRunning) {
			// Informing the system about the loop's start
			AESysFrameStart();
			AEFrameRateControllerStart();

			// Set the delta time
			f32 const dt = 1.f / static_cast<f32>(AEFrameRateControllerGetFrameRate());

			// -=-=-=-=-=-=-=- Update Logic Start -=-=-=-=-=-=-=-

			sceneManager.PreUpdate(dt);
			InputHandler::GetInstance()->Update(dt);
			sceneManager.Update(dt);
			sceneManager.PostUpdate(dt);

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
				bGameRunning = false;
		}

		// free the system
		for (BaseScene* scene : scenes) {
			delete scene;
		}

		InputHandler::Free();
	}
	AESysExit();
}
