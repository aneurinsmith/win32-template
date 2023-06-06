
#pragma once
#include "include.h"
#include "tools/renderer/renderer.h"

extern int _winCount;
extern int _sysHeight;
extern int _sysFrame;

#define APP_NAME L"Template"
#define COL_BODY 0x212021
#define COL_HEAD 0x313031

class defwin {

public:

	defwin() {
		winName = APP_NAME;
		winClass = APP_NAME;
		pos = { CW_USEDEFAULT };
		size = { CW_USEDEFAULT };
		_winCount++;
	}

	~defwin() {
		_winCount--;
	}

	BOOL init() {

		if (!AssignClass(winClass)) throw runtime_error("defwin::init -> AssignClass() Failed");
		if (!AssignWindow(pos, size)) throw runtime_error("defwin::init -> AssignWindow() Failed");

		r.attach(hwnd);

		return true;
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


protected:

	HWND	hwnd,
			prnt;

	RECT	rgn;
	POINT	pos;
	SIZE	size;
	INT		ctrlID;

	DWORD	style,
			exStyle;

	LPCWSTR	winName,
			winClass,
			menuName;

	Renderer r;


	static LRESULT CALLBACK DefProc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

		defwin* data = reinterpret_cast<defwin*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

		switch (msg) {
		case WM_NCCREATE: {

			data = static_cast<defwin*>(reinterpret_cast<LPCREATESTRUCT>(lpm)->lpCreateParams);
			data->hwnd = wnd;
			SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

			break;
		}
		case WM_SIZE: {

			data->rgn.right = GET_X_LPARAM(lpm);
			data->rgn.bottom = GET_Y_LPARAM(lpm);

			data->size = { GET_X_LPARAM(lpm), GET_Y_LPARAM(lpm) };
			data->r.resize_buffer({ 0,0,GET_X_LPARAM(lpm),GET_Y_LPARAM(lpm) });

			RedrawWindow(wnd, NULL, NULL, RDW_INVALIDATE);

			break;
		}
		case WM_MOVE: {

			data->pos = { GET_X_LPARAM(lpm),GET_Y_LPARAM(lpm) };

			break;
		}
		}

		if (data) return data->HandleMessage(wnd, msg, wpm, lpm);
		return DefWindowProc(wnd, msg, wpm, lpm);
	}

	virtual LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) = 0;

	BOOL AssignClass(LPCWSTR _winClass) {
		WNDCLASSEXW wcex = {};

		if (!GetClassInfoExW(GetModuleHandleW(NULL), _winClass, &wcex)) {
			wcex.cbSize = sizeof(wcex);
			wcex.lpfnWndProc = DefProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = GetModuleHandleW(NULL);
			wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
			wcex.hIcon = LoadIcon(GetModuleHandleW(NULL), MAKEINTRESOURCE(ICO_APP));
			wcex.lpszClassName = _winClass;
			wcex.lpszMenuName = menuName;
			wcex.style = CS_BYTEALIGNWINDOW | CS_DBLCLKS;

			if (!RegisterClassExW(&wcex)) {
				return false;
			}
		}

		return true;
	}

	BOOL AssignWindow(POINT p, SIZE s) {
		hwnd = CreateWindowExW(exStyle, winClass, winName, style,
			p.x, p.y, s.cx, s.cy,
			prnt, (HMENU)ctrlID, GetModuleHandleW(NULL), (void*)this);

		if (!hwnd) {
			return false;
		}
		return true;
	}

};
