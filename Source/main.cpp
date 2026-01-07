#include <crtdbg.h> // To check for memory leaks
#include <vector>
#include "AEEngine.h"
#include "AEGraphics.h"
#include "Managers/SceneManager.h"

// Program Entrypoint
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("UI Solo Project Port");

	// Reset the system modules
	AESysReset();
	AEFrameRateControllerInit(120);

	// Initialize fonts
	AEGfxFontSystemStart();

	{
		SceneManager sceneManager;
		// Game Loop
		while (gGameRunning) {
			// Informing the system about the loop's start
			AESysFrameStart();
			AEFrameRateControllerStart();

			// Set the delta time
			f32 const dt = 1.f / static_cast<f32>(AEFrameRateControllerGetFrameRate());

			// -=-=-=-=-=-=-=- Update Logic Start -=-=-=-=-=-=-=-

			sceneManager.Update(dt);

			// -=-=-=-=-=-=-=- Rendering Logic Start -=-=-=-=-=-=-=-

			// Set background color to gray	
			AEGfxSetBackgroundColor(0.3f, 0.3f, 0.3f);
			sceneManager.Render();

			// Informing the system about the loop's end
			AEFrameRateControllerEnd();
			AESysFrameEnd();

			// Basic way to trigger exiting the application
			// when ESCAPE is hit or when the window is closed
			if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
				gGameRunning = 0;
		}
		// free the system
	}
	AESysExit();
}
