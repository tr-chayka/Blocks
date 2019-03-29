#include "utils.h"

#include <commctrl.h>
#include <cstdlib>
#include <string>

namespace Game
{
	HWND CreateRenderBox(HWND hParent, const Rect &rect)
	{
		return CreateWindowExW(
			0, WC_STATICW, nullptr,
			WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
			rect.x, rect.y, rect.w, rect.h,
			hParent,
			0,
			GetModuleHandleW(0),
			0);
	}

	HWND CreateCaption(LPCWSTR text, HWND hParent, const Rect &rect)
	{
		return CreateWindowExW(
			0, WC_STATICW, text,
			WS_CHILD | WS_VISIBLE,
			rect.x, rect.y, rect.w, rect.h,
			hParent,
			0,
			GetModuleHandleW(0),
			0);
	}

	HWND CreateGroupBox(LPCWSTR text, HWND hParent, const Rect &rect)
	{
		return CreateWindowExW(
			0, WC_BUTTONW, text,
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			rect.x, rect.y, rect.w, rect.h,
			hParent,
			0,
			GetModuleHandleW(0),
			0);
	}

	void SetValue(HWND hWnd, UINT value)
	{
		std::wstring text = std::to_wstring(value);
		SendMessageW(hWnd, WM_SETTEXT, 0, reinterpret_cast<LPARAM>(text.c_str()));
	}

	Figure Figure::operator = (const Figure &figure)
	{
		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
			points[i] = figure.points[i];

		return figure;
	}

	Point operator + (const Point &p1, const Point &p2)
	{
		return
		{
			p1.x + p2.x,
			p1.y + p2.y,
		};
	}

	Point rotatePoint(const Point &p)
	{
		return { -p.y, p.x };
	}

	Figure rotateFigure(const Figure &f)
	{
		Figure result;

		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
			result.points[i] = rotatePoint(f.points[i]);

		return result;
	}

	Figure getFigure(UINT index)
	{
		Figure result;

		for (UINT i = 0; i < POINT_IN_FIGURE; i++)
			result.points[i] = FIGURE_LIST[index][i];

		return result;
	}

	UINT random()
	{
		return 1 + rand() % FIGURE_COUNT;
	}
}
