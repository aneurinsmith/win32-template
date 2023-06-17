
#include "window.h"

auto window_console = LOG::console_logger("window", 30);

Window::Window() {
	window_console.print(INFO, "Window::Window()");
	winClass = L"template";
	winName = L"template";
	style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

	c.open(CSIDL_APPDATA, "Template\\config.ini", ios::in);
	pos = {
		c.isSet("window", "x") ? c.getVal("window", "x") : 40,
		c.isSet("window", "y") ? c.getVal("window", "y") : 40
	};
	size = {
		c.isSet("window", "cx") ? c.getVal("window", "cx") : 600,
		c.isSet("window", "cy") ? c.getVal("window", "cy") : 400
	};
	fs = c.isSet("window", "fs") ? c.getVal("window", "fs") : 0;
	c.close();
}

BOOL Window::init() {
	window_console.print(INFO, "Window::init()");

	defwin::init();

	BOOL value = TRUE;
	DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

	maximize(fs);
	display();
	
	return true;
}



LRESULT Window::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {
	window_console.print(TRACE, "Window::HandleMessage(HWND ",wnd,", UINT ",msg,", WPARAM ",wpm,", LPARAM ",lpm,")");

	BOOL callDef = true;
	BOOL callDWM = false;
	LRESULT lRet = 0;

	DwmIsCompositionEnabled(&callDWM);
	if (callDWM) {
		lRet = HandleDWM(wnd, msg, wpm, lpm, &callDef);
	}
	if (callDef) {
		lRet = HandleDef(wnd, msg, wpm, lpm);
	}

	return lRet;
}

LRESULT Window::HandleDef(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

	switch (msg) {

		// Launch the core app
		case WM_CREATE:
		{
			core._window = this;
			core.init();

			break;
		}

		// Delegate to core app if it is launched and pass window messages to the caption
		case WM_SIZE:
		{
			if (core._window) core.handle_resize();
			SendMessage(core.caption.hwnd, msg, wpm, lpm);

			break;
		}

		// Draw background color
		case WM_PAINT:
		{
			r.resize_buffer({ 0,0,size.cx,size.cy });
			r.begin_draw();
			r.resize_buffer({ 0,1,size.cx,size.cy });
			r.draw_rect({ 0,0,size.cx,size.cy }, COL_BODY);
			r.draw_rect({ 0,0,size.cx,1 }, 0x000000);
			r.end_draw();

			break;
		}

		// Destroy, but I dont need to tell you that
		case WM_DESTROY:
		{
			WINDOWPLACEMENT wp = {};
			GetWindowPlacement(wnd, &wp);

			c.open(CSIDL_APPDATA, "Template\\config.ini", ios::in | ios::out);
			c.setVal("window", "x", wp.rcNormalPosition.left);
			c.setVal("window", "y", wp.rcNormalPosition.top);
			c.setVal("window", "cx", wp.rcNormalPosition.right - wp.rcNormalPosition.left);
			c.setVal("window", "cy", wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
			c.setVal("window", "fs", wp.flags == 2 ? 1 : 0);
			c.close();

			display(false);
			PostQuitMessage(0);

			break;
		}

		default:
		{
			return DefWindowProc(wnd, msg, wpm, lpm);
		}
	}
	return 0;
}

LRESULT Window::HandleDWM(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm, BOOL* callDef) {

	LRESULT lRet = 0;
	BOOL _callDef = true;

	_callDef = !DwmDefWindowProc(wnd, msg, wpm, lpm, &lRet);

	switch (msg) {

		// Sets the caption height from the window theme and extends frame into client area
		case WM_CREATE:
		{
			_sysHeight = 100;
			_sysFrame = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
			rgn.top = _sysHeight;

			MARGINS m = { 0,0,rgn.top,0 };
			DwmExtendFrameIntoClientArea(wnd, &m);

			resize({ 0 }, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

			break;
		}

		// Removes the default window but keeps the aero shadow
		case WM_NCCALCSIZE:
		{
			if (wpm == TRUE) {
				NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lpm);

				sz->rgrc[0].left += _sysFrame;
				sz->rgrc[0].right -= _sysFrame;
				sz->rgrc[0].bottom -= _sysFrame;
			}
			lRet = 0;
			_callDef = false;

			break;
		}

		// Handles non-client hittest for the caption area, else handled by the DefWindowProc
		case WM_NCHITTEST:
		{
			BOOL handled = DwmDefWindowProc(wnd, msg, wpm, lpm, &lRet);
			if (!handled) {
				POINT mouse = { GET_X_LPARAM(lpm), GET_Y_LPARAM(lpm) };
				ScreenToClient(hwnd, &mouse);

				if (mouse.y < _sysHeight && mouse.x < rgn.right && mouse.x > 0) {
					if (mouse.y < _sysFrame) lRet = HTTOP;
					else if (mouse.y > rgn.bottom) lRet = HTBOTTOM;
					else lRet = HTCAPTION;
				}
				else {
					lRet = DefWindowProc(wnd, msg, wpm, lpm);
				}
			}
			_callDef = false;

			break;
		}
	}

	*callDef = _callDef;
	return lRet;
}

