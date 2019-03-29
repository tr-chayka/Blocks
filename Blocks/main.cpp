#include "manifest.h"
#include "mainwindow.h"

#pragma comment(lib, "d2d1.lib")

using namespace Game;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	MainWindow mainWindow(hInstance);

	while (true)
	{
		MSG msg = { 0 };
		if (PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		else
			mainWindow.render();

		if (msg.message == WM_QUIT)
			return msg.lParam;
	}
}