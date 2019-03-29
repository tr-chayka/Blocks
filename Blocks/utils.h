#pragma once
#include "const.h"

#include <windows.h>

namespace Game
{
	struct Figure
	{
		Point points[POINT_IN_FIGURE];

		Figure operator = (const Figure &figure);
	};

	HWND CreateRenderBox(HWND hParent, const Rect &rect);
	HWND CreateCaption(LPCWSTR text, HWND hParent, const Rect &rect);
	HWND CreateGroupBox(LPCWSTR text, HWND hParent, const Rect &rect);

	void SetValue(HWND hWnd, UINT value);

	Point operator + (const Point &p1, const Point &p2);
	Point rotatePoint(const Point &p);
	Figure rotateFigure(const Figure &f);
	Figure getFigure(UINT index);

	UINT random();
}