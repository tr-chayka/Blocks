#pragma once
#include <d2d1.h>

namespace Game
{
	struct Rect
	{
		UINT x;
		UINT y;
		UINT w;
		UINT h;
	};

	struct Point
	{
		int x;
		int y;
	};

	const LPCWSTR MAIN_WINDOW_CLASS = L"BlocksMainWindowClass";
	const LPCWSTR MAIN_WINDOW_TITLE = L"Blocks";
	const LPCWSTR NEXT_BOX_CAPTION = L"Next";
	const LPCWSTR SCORE_BOX_CAPTION = L"Score";
	const LPCWSTR LINES_CAPTION = L"Lines";
	const LPCWSTR LEVEL_CAPTION = L"Level";
	const LPCWSTR SCORE_CAPTION = L"Score";

	const LPCWSTR TEXT_ZERO = L"0";

	const UINT MAIN_WINDOW_WIDTH = 520;
	const UINT MAIN_WINDOW_HEIGHT = 665;

	const UINT GAME_FIELD_WIDTH = 10;
	const UINT GAME_FIELD_HEIGHT = 20;
	const UINT FIGURE_COUNT = 7;
	const UINT POINT_IN_FIGURE = 4;

	const UINT MAX_DELAY = 500;
	const UINT MIN_DELAY = 180;
	const UINT DELAY_DELTA = 20;
	const UINT FULL_SPEED_DELAY = 50;

	const UINT MIN_SCORE_PER_LINE = 5;
	const UINT SCORE_PER_LEVEL_RATIO = 2;
	const UINT MIN_LINES_FOR_LEVEL = 10;
	const UINT LINES_FOR_LEVEL_RATIO = 2;
	
	const UINT SCORE_RATIO_X[4] = {1, 3, 5, 6};

	const D2D1_COLOR_F FIGURE_COLOR_LIST[FIGURE_COUNT] =
	{
		{ 0.78f, 0.00f, 0.00f, 1.0f },
		{ 0.00f, 0.78f, 0.39f, 1.0f },
		{ 0.39f, 0.00f, 0.78f, 1.0f },
		{ 0.00f, 0.29f, 0.60f, 1.0f },
		{ 1.00f, 0.50f, 0.00f, 1.0f },
		{ 0.80f, 0.80f, 0.00f, 1.0f },
		{ 0.00f, 0.58f, 0.58f, 1.0f },
	};

	const D2D1_COLOR_F BACKGROUND_COLOR = { 0.95f, 0.95f, 0.95f, 1.0f };
	const D2D1_COLOR_F GAMEOVER_COLOR = { 0.70f, 0.70f, 0.70f, 1.0f };
	const D2D1_COLOR_F GAMEOVER_TEXT_COLOR = { 0.50f, 0.00f, 0.00f, 1.0f };
	const D2D1_COLOR_F GRID_COLOR = { 0.70f, 0.70f, 0.70f, 1.0f };
	const D2D1_COLOR_F TRANSPARENT_COLOR = { 0.20f, 0.20f, 0.20f, 0.2f };

	const Rect NEXT_BOX_RECT = { 320, 10, 170, 200 };
	const Rect SCORE_BOX_RECT = { 320, 220, 170, 200 };
	const Rect NEXT_RECT = { 25, 40, 120, 120 };
	const Rect FIELD_RECT = { 10, 10, 300, 600 };
	
	const Rect LEVEL_TEXT_RECT = { 10, 60, 150, 30 };
	const Rect LINES_TEXT_RECT = { 10, 110, 150, 30 };
	const Rect SCORE_TEXT_RECT = { 10, 160, 150, 30 };

	const Rect LEVEL_CAPTION_RECT = { 10, 40, 100, 20 };
	const Rect LINES_CAPTION_RECT = { 10, 90, 100, 20 };
	const Rect SCORE_CAPTION_RECT = { 10, 140, 100, 20 };

	const float CELL_SIZE = 30.0f;
	const float CELL_PADDING = 0.5f;

	const Point FIGURE_LIST[FIGURE_COUNT][POINT_IN_FIGURE] = 
	{
		{ {+0, +0}, {+1, +0}, {+0, +1}, {+1, +1} },
		{ {-1, +0}, {+0, +0}, {+0, +1}, {+1, +1} },
		{ {-1, +1}, {+0, +1}, {+0, +0}, {+1, +0} },
		{ {-1, +0}, {-1, +1}, {+0, +0}, {+1, +0} },
		{ {-1, +0}, {+0, +0}, {+1, +0}, {+1, +1} },
		{ {-1, +0}, {+0, +0}, {+1, +0}, {+0, -1} },
		{ {+0, -1}, {+0, +0}, {+0, +1}, {+0, +2} },
	};

	const Point START_POSITION[FIGURE_COUNT] = 
	{
		{ GAME_FIELD_WIDTH / 2, 0 },
		{ GAME_FIELD_WIDTH / 2, 0 },
		{ GAME_FIELD_WIDTH / 2, 0 },
		{ GAME_FIELD_WIDTH / 2, 0 },
		{ GAME_FIELD_WIDTH / 2, 0 },
		{ GAME_FIELD_WIDTH / 2, 1 },
		{ GAME_FIELD_WIDTH / 2, 1 },
	};
}
