#pragma once
#include "const.h"
#include "game.h"

#include <d2d1.h>
#include <d2d1helper.h>
#include <windows.h>

namespace Game
{
	class Gui
	{
	public:
		Gui(HWND hWnd);

		void render();

		void onTimer();
		void onLostFocus();
		void onKeyDown(WPARAM key);
		void onKeyUp(WPARAM key);

	private:
		void renderField();
		void renderNextBox();
		void update();

	private:
		HWND m_hParentWnd;
		HWND m_hFieldRenderBox;
		HWND m_hNextRenderBox;
		HWND m_hScoreText;
		HWND m_hLinesText;
		HWND m_hLevelText;

		ID2D1Factory *m_pFactory;
		ID2D1HwndRenderTarget *m_pFieldCanvas;
		ID2D1HwndRenderTarget *m_pNextCanvas;

		ID2D1SolidColorBrush *m_pFieldBrushList[FIGURE_COUNT];
		ID2D1SolidColorBrush *m_pNextBrushList[FIGURE_COUNT];

		ID2D1SolidColorBrush *m_pGridBrush;
		ID2D1SolidColorBrush *m_pGameOverBrush;
		ID2D1SolidColorBrush *m_pTransparentBrush;

		Game m_game;
		UINT m_timerDelay;
	};
}