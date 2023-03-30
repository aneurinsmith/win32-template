
#include "window.h" 

int Window::sys_height = 0;
int Window::sys_frame = 0;

Window::Window() {
	winClass = APP_NAME;
	winName = APP_NAME;
	style = WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
}

LRESULT Window::init() {

	defwin::init();

	BOOL value = TRUE;
	DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

	WINDOWPLACEMENT wp;
	GetWindowPlacement(hwnd, &wp);
	wp.rcNormalPosition = core.params.get_rect();
	wp.showCmd = core.params.get_fs() ? SW_SHOWMAXIMIZED : SW_SHOWNORMAL;
	SetWindowPlacement(hwnd, &wp);

	return true;
}

LRESULT Window::HandleDef(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {
	
	switch (msg)
	{

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
			if(core._window) core.handle_resize();
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
			core.save();
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
			Window::sys_height = 120;
			Window::sys_frame = GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
			rgn.top = Window::sys_height;

			MARGINS m = { 0,0,rgn.top,0 };
			DwmExtendFrameIntoClientArea(wnd, &m);

			resize({ 0 }, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

			break;
		}

		// Removes the default window but keeps the aero shadow
		case WM_NCCALCSIZE: 
		{
			if (wpm == TRUE) 
			{
				NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lpm);

				sz->rgrc[0].left += Window::sys_frame;
				sz->rgrc[0].right -= Window::sys_frame;
				sz->rgrc[0].bottom -= Window::sys_frame;
			}
			lRet = 0;
			_callDef = false;

			break;
		}

		// Handles non-client hittest for the caption area, else handled by the DefWindowProc
		case WM_NCHITTEST: 
		{
			BOOL handled = DwmDefWindowProc(wnd, msg, wpm, lpm, &lRet);
			if (!handled)
			{
				POINT mouse = { GET_X_LPARAM(lpm), GET_Y_LPARAM(lpm) };
				ScreenToClient(hwnd, &mouse);

				if (mouse.y < sys_height && mouse.x < rgn.right && mouse.x > 0) 
				{
					if (mouse.y < Window::sys_frame) lRet = HTTOP;
					else if (mouse.y > rgn.bottom) lRet = HTBOTTOM;
					else lRet = HTCAPTION;
				}
				else 
				{
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


LRESULT Window::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {
	
	BOOL callDef = true;
	BOOL callDWM = false;
	LRESULT lRet = 0;

	DwmIsCompositionEnabled(&callDWM);
	if (callDWM) 
	{
		lRet = HandleDWM(wnd, msg, wpm, lpm, &callDef);
	}
	if (callDef) 
	{
		lRet = HandleDef(wnd, msg, wpm, lpm);
	}

	return lRet;
}