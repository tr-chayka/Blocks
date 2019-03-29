#pragma once
#include "gui.h"

#include <memory>

namespace Game
{
	class MainWindow
	{
	public:
		MainWindow(HINSTANCE hInsntace);
		void render();

	private:
		HWND m_hWnd;

		static LRESULT CALLBACK staticWndProc(HWND, UINT, WPARAM, LPARAM);
		LRESULT wndProc(UINT, WPARAM, LPARAM);

		std::unique_ptr<Gui> m_pGui;
	};
}