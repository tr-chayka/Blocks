#include "gui.h"
#include "utils.h"

namespace Game
{
	Gui::Gui(HWND hWnd) : m_hParentWnd(hWnd)
	{
		// Create controls

		HWND hNextBox = CreateGroupBox(NEXT_BOX_CAPTION, m_hParentWnd, NEXT_BOX_RECT);
		HWND hScoreBox = CreateGroupBox(SCORE_BOX_CAPTION, m_hParentWnd, SCORE_BOX_RECT);

		if (hNextBox == 0 || hScoreBox == 0)
			throw "Can not create gui";

		m_hNextRenderBox = CreateRenderBox(hNextBox, NEXT_RECT);
		m_hFieldRenderBox = CreateRenderBox(m_hParentWnd, FIELD_RECT);

		if (m_hNextRenderBox == 0 || m_hFieldRenderBox == 0)
			throw "Can not create gui";

		m_hLevelText = CreateCaption(TEXT_ZERO, hScoreBox, LEVEL_TEXT_RECT);
		m_hLinesText = CreateCaption(TEXT_ZERO, hScoreBox, LINES_TEXT_RECT);
		m_hScoreText = CreateCaption(TEXT_ZERO, hScoreBox, SCORE_TEXT_RECT);

		if (m_hLevelText == 0 || m_hLinesText == 0 || m_hScoreText == 0)
			throw "Can not create gui";

		HWND hLevelCaption = CreateCaption(LEVEL_CAPTION, hScoreBox, LEVEL_CAPTION_RECT);
		HWND hLinesCaption = CreateCaption(LINES_CAPTION, hScoreBox, LINES_CAPTION_RECT);
		HWND hScoreCaption = CreateCaption(SCORE_CAPTION, hScoreBox, SCORE_CAPTION_RECT);

		if (hLevelCaption == 0 || hLinesCaption == 0 || hScoreCaption == 0)
			throw "Can not create gui";

		// Set default font

		WPARAM DEFAULT_FONT = reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT));

		SendMessageW(hNextBox, WM_SETFONT, DEFAULT_FONT, 0);
		SendMessageW(hScoreBox, WM_SETFONT, DEFAULT_FONT, 0);

		SendMessageW(hLevelCaption, WM_SETFONT, DEFAULT_FONT, 0);
		SendMessageW(hLinesCaption, WM_SETFONT, DEFAULT_FONT, 0);
		SendMessageW(hScoreCaption, WM_SETFONT, DEFAULT_FONT, 0);

		// Init Direct2D

		if (D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pFactory) != S_OK)
			throw "Can not initialize Direct2D";

		D2D1_SIZE_U FIELD_SIZE = { FIELD_RECT.w, FIELD_RECT.h };
		D2D1_SIZE_U NEXT_SIZE = { NEXT_RECT.w, NEXT_RECT.h };

		if
		(
			S_OK != m_pFactory->CreateHwndRenderTarget
			(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hFieldRenderBox, FIELD_SIZE),
				&m_pFieldCanvas
			)

			||

			S_OK != m_pFactory->CreateHwndRenderTarget
			(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(m_hNextRenderBox, NEXT_SIZE),
				&m_pNextCanvas
			)
		)
			throw "Can not create render target";


		for (UINT i = 0; i < FIGURE_COUNT; i++)
		{
			if
			(
				S_OK != m_pFieldCanvas->CreateSolidColorBrush(FIGURE_COLOR_LIST[i], &m_pFieldBrushList[i]) ||
				S_OK != m_pNextCanvas->CreateSolidColorBrush(FIGURE_COLOR_LIST[i], &m_pNextBrushList[i])
			)
				throw "Can not create brushes";
		}

		if
		(
			S_OK != m_pFieldCanvas->CreateSolidColorBrush(GRID_COLOR, &m_pGridBrush) ||
			S_OK != m_pFieldCanvas->CreateSolidColorBrush(GAMEOVER_COLOR, &m_pGameOverBrush) ||
			S_OK != m_pFieldCanvas->CreateSolidColorBrush(TRANSPARENT_COLOR, &m_pTransparentBrush)
		)
			throw "Can not create brushes";

		m_pFieldCanvas->SetDpi(96.0f, 96.0f);
		m_pNextCanvas->SetDpi(96.0f, 96.0f);
	}

	void Gui::render()
	{
		m_pFieldCanvas->BeginDraw();
		{
			m_pFieldCanvas->Clear(BACKGROUND_COLOR);
			renderField();
		}
		m_pFieldCanvas->EndDraw();

		m_pNextCanvas->BeginDraw();
		{
			m_pNextCanvas->Clear(BACKGROUND_COLOR);
			renderNextBox();
		}
		m_pNextCanvas->EndDraw();
	}

	void Gui::renderField()
	{
		for (UINT i = 0; i <= GAME_FIELD_WIDTH; i++)
		{
			D2D1_POINT_2F start = { i * CELL_SIZE, 0.0f };
			D2D1_POINT_2F end = { i * CELL_SIZE, FIELD_RECT.h };

			m_pFieldCanvas->DrawLine(start, end, m_pGridBrush);
		}

		for (UINT j = 0; j <= GAME_FIELD_HEIGHT; j++)
		{
			D2D1_POINT_2F start = { 0.0f, j * CELL_SIZE };
			D2D1_POINT_2F end = { FIELD_RECT.w, j * CELL_SIZE };

			m_pFieldCanvas->DrawLine(start, end, m_pGridBrush);
		}

		if (!m_game.isStarted() && !m_game.isGameOver())
			return;

		const bool isGameOver = m_game.isGameOver();

		for (UINT i = 0; i < GAME_FIELD_WIDTH; i++)
			for (UINT j = 0; j < GAME_FIELD_HEIGHT; j++)
			{
				UINT value = m_game.get(i, j);

				if (value > 0)
				{
					ID2D1SolidColorBrush *brush = isGameOver ?
						m_pGameOverBrush :
						m_pFieldBrushList[value - 1];

					D2D1_RECT_F cell =
					{
						i * CELL_SIZE + CELL_PADDING,
						j * CELL_SIZE + CELL_PADDING,
						(i + 1) * CELL_SIZE - CELL_PADDING,
						(j + 1) * CELL_SIZE - CELL_PADDING,
					};

					m_pFieldCanvas->FillRectangle(cell, brush);
				}
			}

		if (m_game.isPause())
		{
			D2D1_RECT_F field =
			{
				0,
				0,
				FIELD_RECT.w,
				FIELD_RECT.h,
			};

			m_pFieldCanvas->FillRectangle(field, m_pTransparentBrush);
		}
	}

	void Gui::renderNextBox()
	{
		if (!m_game.isStarted())
			return;

		UINT nextFigureIndex = m_game.getNextFigureIndex();

		if (nextFigureIndex == 0 || nextFigureIndex > FIGURE_COUNT)
			return;

		Figure nextFigure = getFigure(nextFigureIndex - 1);

		const float CELL_HALF_SIZE = CELL_SIZE * 0.5f;
		const float cx = NEXT_RECT.w * 0.5f;
		const float cy = NEXT_RECT.h * 0.5f;

		float fcx = 0.0f;
		float fcy = 0.0f;

		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
		{
			fcx += nextFigure.points[i].x;
			fcy += nextFigure.points[i].y;
		}

		fcx /= POINT_IN_FIGURE;
		fcy /= POINT_IN_FIGURE;

		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
		{
			float centerX = cx + (nextFigure.points[i].x - fcx) * CELL_SIZE;
			float centerY = cy + (nextFigure.points[i].y - fcy) * CELL_SIZE;

			D2D1_RECT_F cell =
			{
				centerX - CELL_HALF_SIZE + CELL_PADDING,
				centerY - CELL_HALF_SIZE + CELL_PADDING,
				centerX + CELL_HALF_SIZE - CELL_PADDING,
				centerY + CELL_HALF_SIZE - CELL_PADDING,
			};

			m_pNextCanvas->FillRectangle(cell, m_pNextBrushList[nextFigureIndex - 1]);
		}
	}

	void Gui::onTimer()
	{
		if (m_game.step())
		{
			update();

			if (m_game.delay() != m_timerDelay)
			{
				m_timerDelay = m_game.delay();
				SetTimer(m_hParentWnd, 0, m_timerDelay, 0);
			}
		}
	}

	void Gui::onKeyUp(WPARAM key)
	{
		switch (key)
		{
			case VK_RETURN:
				m_game.newGame();
				m_timerDelay = m_game.delay();
				SetTimer(m_hParentWnd, 0, m_timerDelay, 0);
				update();
				break;

			case VK_SPACE:
				if (m_game.isPause())
					m_game.resume();
				else
					m_game.pause();
				break;

			case VK_DOWN:
				if (m_game.isFullSpeed())
				{
					m_game.setFullSpeed();
					SetTimer(m_hParentWnd, 0, m_timerDelay, 0);
				}
				break;
		}
	}

	void Gui::onKeyDown(WPARAM key)
	{
		switch (key)
		{
			case VK_UP:
				m_game.rotate();
				break;

			case VK_LEFT:
				m_game.moveLeft();
				break;

			case VK_RIGHT:
				m_game.moveRight();
				break;

			case VK_DOWN:
				if (!m_game.isFullSpeed())
				{
					m_game.setFullSpeed();
					SetTimer(m_hParentWnd, 0, FULL_SPEED_DELAY, 0);
				}
				break;
		}
	}


	void Gui::onLostFocus()
	{
		m_game.pause();
	}

	void Gui::update()
	{
		SetValue(m_hLevelText, m_game.level());
		SetValue(m_hLinesText, m_game.lines());
		SetValue(m_hScoreText, m_game.score());
	}
}


/*void Gui::render()
{
	m_pFieldRenderTarget->BeginDraw();
	m_pFieldRenderTarget->Clear(backgroundColor);

	for (int i = 0; i <= 10; i++)
		m_pFieldRenderTarget->DrawLine
		(
			{ i * 30.0f, 0.0f },
			{ i * 30.0f, 600.0f },
			m_pGridBrush
		);
		
	for (int i = 0; i <= 20; i++)
		m_pFieldRenderTarget->DrawLine
		(
			{ 0.0f, i * 30.0f },
			{ 300.0f, i * 30.0f },
			m_pGridBrush
		);

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 20; j++)
		{
			int c = m_game.get(i, j);
		
			if (c > 0)
			{
				D2D1_RECT_F rect;
				rect.left = i * 30.f + 0.5f;
				rect.top = j * 30.f + 0.5f;
				rect.right = rect.left + 29.0f;
				rect.bottom = rect.top + 29.0f;

				if (m_game.gameOver())
					m_pFieldRenderTarget->FillRectangle(rect, m_pGameOverBrush);
				else
					m_pFieldRenderTarget->FillRectangle(rect, m_pBrushList[c - 1]);
			}
		}

	if (m_pause)
	{
		D2D1_RECT_F rect;
		rect.left = 0.0f;
		rect.right = 300.0f;
		rect.top = 0.0f;
		rect.bottom = 600.0f;

		m_pFieldRenderTarget->FillRectangle(rect, m_pTransparentBrush);
	}

	m_pFieldRenderTarget->EndDraw();

	m_pNextRenderTarget->BeginDraw();
	m_pNextRenderTarget->Clear(backgroundColor);
	
	unsigned index = m_game.nextFigure();

	if (index > 0)
	{
		int geometry[8];
		m_game.nextFigureGeometry(geometry);
	
		float centerX = 0.0f;
		float centerY = 0.0f;

		for (int i = 0; i < 4; i++)
		{
			int x = geometry[i * 2];
			int y = geometry[i * 2 + 1];

			centerX += (float)x;
			centerY += (float)y;
		}

		centerX /= 4;
		centerY /= 4;

		float boxCenterX = 60.0f;
		float boxCenterY = 60.0f;

		for (int i = 0; i < 4; i++)
		{
			float x = (float)geometry[i * 2];
			float y = (float)geometry[i * 2 + 1];

			float dx = x - centerX;
			float dy = y - centerY;

			D2D1_RECT_F rect;
			rect.left = boxCenterX + (dx * 30.0f) - 14.5f;
			rect.right = rect.left + 29.0f;
			rect.top = boxCenterY + (dy * 30.0f) - 14.5f;
			rect.bottom = rect.top + 29.0f;

			m_pNextRenderTarget->FillRectangle(rect, m_pNextBrushList[index - 1]);
		}
	}
	
	m_pNextRenderTarget->EndDraw();
}

void Gui::onKeyPress(DWORD key)
{
	switch (key)
	{
		case VK_UP:
			if (!m_pause)
				m_game.rotate();
			break;

		case VK_LEFT:
			if (!m_pause)
				m_game.left();
			break;

		case VK_RIGHT:
			if (!m_pause)
				m_game.right();
			break;

		case VK_RETURN:
			m_game.start();
			updateScore(m_game.score());
			updateLines(m_game.lines());
			updateLevel(m_game.level());
			break;
	}
}

void Gui::onTimer()
{
	if (m_pause)
		return;

	if (m_game.step())
	{
		updateScore(m_game.score());
		updateLines(m_game.lines());
		updateLevel(m_game.level());
	}
}

void Gui::pause()
{
	if (m_game.active())
		m_pause = !m_pause;
}*/