#include <Windows.h>
#include "AEEngine.h"
#include "AEGraphics.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    AESysInit(hInstance, 1, 1600, 900, _DEBUG, 60, true, NULL);
    AESysSetWindowTitle("Alpha Engine");

    while (AESysDoesWindowExist()) {
        AESysFrameStart();

        AESysFrameEnd();
    }
    AESysExit();
    return 0;
}