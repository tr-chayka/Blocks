#include "const.h"
#include "mainwindow.h"
#include "resource.h"

namespace Game
{
	MainWindow::MainWindow(HINSTANCE hInstance)
	{
		WNDCLASSW wc = { 0 };
		wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
		wc.hInstance = hInstance;
		wc.lpfnWndProc = staticWndProc;
		wc.lpszClassName = MAIN_WINDOW_CLASS;
		wc.style = CS_VREDRAW | CS_HREDRAW;
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION_ICON));
		wc.hCursor = LoadCursor(0, IDC_ARROW);

		if (!RegisterClassW(&wc))
			throw L"Can not register main window class";

		m_hWnd = CreateWindowExW
		(
			0,
			MAIN_WINDOW_CLASS,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			MAIN_WINDOW_WIDTH,
			MAIN_WINDOW_HEIGHT,
			0,
			0,
			hInstance,
			this
		);

		if (m_hWnd == 0)
			throw L"Can not create main window";

		m_pGui = std::make_unique<Gui>(m_hWnd);
	}

	LRESULT CALLBACK MainWindow::staticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NCCREATE)
		{
			LPCREATESTRUCTW pCreateStruct = reinterpret_cast<LPCREATESTRUCTW>(lParam);
			MainWindow *pWindow = reinterpret_cast<MainWindow *>(pCreateStruct->lpCreateParams);

			if (pWindow != nullptr)
				SetWindowLongW(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(pWindow));
		}
		else
		{
			MainWindow *pWindow = reinterpret_cast<MainWindow *>(GetWindowLongW(hWnd, GWL_USERDATA));

			if (pWindow != nullptr && pWindow->m_hWnd == hWnd)
				return pWindow->wndProc(msg, wParam, lParam);
		}

		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}

	LRESULT MainWindow::wndProc(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
		
			case WM_KEYDOWN:
				m_pGui->onKeyDown(wParam);
				return 0;

			case WM_KEYUP:
				m_pGui->onKeyUp(wParam);
				return 0;

			case WM_TIMER:
				m_pGui->onTimer();
				return 0;

			case WM_KILLFOCUS:
				m_pGui->onLostFocus();
				return 0;
		}

		return DefWindowProcW(m_hWnd, msg, wParam, lParam);
	}

	void MainWindow::render()
	{
		m_pGui->render();
	}
}