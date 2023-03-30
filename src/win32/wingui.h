#pragma once
#include "include.h"
#include "tools/renderer/renderer.h"


template <class win_temp>
class defwin {
protected:
	HWND	hwnd, 
			prnt;

	RECT	rgn = { 0,0,0,0 };
	POINT	pos = { 0, 0 };
	SIZE	size = { 800,600 };
	INT		ctrlID;

	DWORD	style,
			exStyle;

	LPCWSTR	winName,
			menuName,
			winClass;

	Renderer r;

	static LRESULT CALLBACK DefProc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

		win_temp* win_data = reinterpret_cast<win_temp*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg) {
			case WM_NCCREATE: {

				win_temp* win_data = static_cast<win_temp*>((reinterpret_cast<LPCREATESTRUCT>(lpm))->lpCreateParams);
				win_data->hwnd = wnd;
				SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(win_data));

				break;
			}
			case WM_SIZE: {

				win_data->rgn.right = GET_X_LPARAM(lpm);
				win_data->rgn.bottom = GET_Y_LPARAM(lpm);

				win_data->size = { GET_X_LPARAM(lpm),GET_Y_LPARAM(lpm) };
				win_data->r.resize_buffer({ 0,0,GET_X_LPARAM(lpm),GET_Y_LPARAM(lpm) });

				RedrawWindow(wnd, NULL, NULL, RDW_INVALIDATE);

				break;
			}
			case WM_MOVE: {

				win_data->pos = { GET_X_LPARAM(lpm), GET_Y_LPARAM(lpm) };

				break;
			}
			case WM_CLOSE: {

				//PostQuitMessage(0);

				break;
			}
		}
		return win_data->HandleMessage(wnd, msg, wpm, lpm);
	}

	BOOL AssignClass(LPCWSTR winClass) {
		WNDCLASSEXW wcex = {};
		wcex.cbSize = sizeof(wcex);
		wcex.lpfnWndProc = DefProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = CreateSolidBrush(COL_BODY);
		wcex.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO_APP));
		wcex.lpszClassName = winClass;
		wcex.lpszMenuName = menuName;
		wcex.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;
		
		if (!GetClassInfoExW(GetModuleHandle(NULL), winClass, &wcex))
		{
			if (!RegisterClassExW(&wcex)) return false;
		}
		return true;
	}

	BOOL AssignWindow(POINT p, SIZE s) {
		hwnd = CreateWindowExW(exStyle, winClass, winName, style,
			p.x, p.y, s.cx, s.cy,
			prnt, (HMENU)ctrlID, GetModuleHandle(NULL), (void*)this);

		if (!hwnd) return false;
		return true;
	}

public:

	defwin() = default;
	~defwin() = default;

	LRESULT init() {

		// Assign to application
		if (!AssignClass(winClass)) throw std::runtime_error("\"window::Create -> RegisterClass()\" Failed");
		if (!AssignWindow(pos, size)) throw std::runtime_error("\"window::Create -> CreateWindowExW()\" Failed");

		r.attach(hwnd);
	}

	void display(bool show = true) const {
		::ShowWindow(hwnd, show ? SW_SHOW : SW_HIDE);
	}

	void maximize(bool max = true) const {
		::ShowWindow(hwnd, max ? SW_SHOWMAXIMIZED : SW_RESTORE);
	}

	void resize(RECT rc, UINT flags) {
		pos = { rc.left, rc.top };
		::SetWindowPos(hwnd, nullptr, rc.left, rc.top, rc.right, rc.bottom, flags);
	}
};
