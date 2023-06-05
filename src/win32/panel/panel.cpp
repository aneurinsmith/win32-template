
#include "panel.h"

Panel::Panel() {
	winClass = L"panel";
	winName = L"panel";
	style = WS_CHILD;
}

LRESULT Panel::init(HWND parent) {

	prnt = parent;
	defwin::init();

	resize({ 0 }, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
	display(true);

	return true;
}


LRESULT Panel::HandleDef(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

	switch (msg)
	{
		case WM_LBUTTONDOWN:
		{
			display(false);
			if(docked) {
				docked = false;

				SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
				SetParent(hwnd, NULL);

				BOOL value = TRUE;
				DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

				MARGINS m = { 0,0,24,0 };
				DwmExtendFrameIntoClientArea(hwnd, &m);

				display(true);
			}
			else {
				docked = true;

				SetWindowLong(hwnd, GWL_STYLE, WS_CHILD);
				SetParent(hwnd, prnt);

				display(true);
			}
			break;
		}

		case WM_PAINT:
		{
			r.resize_buffer({ 0,0,size.cx,size.cy });
			r.begin_draw();
			r.resize_buffer({ 0,1,size.cx,size.cy });
			r.draw_rect({ 0,0,size.cx,size.cy }, 0x313031);
			if(!docked) r.draw_rect({ 0,0,size.cx,1 }, 0x000000);
			r.end_draw();
			break;
		}

		default:
		{
			return DefWindowProc(wnd, msg, wpm, lpm);
		}
	}
	return 0;
}


LRESULT Panel::HandleDWM(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm, BOOL* callDef) {

	LRESULT lRet = 0;
	BOOL _callDef = true;

	if (this && !docked) {

		_callDef = !DwmDefWindowProc(wnd, msg, wpm, lpm, &lRet);

		switch (msg) {
			// Sets the caption height from the window theme and extends frame into client area
			case WM_CREATE:
			{
				SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
				SetParent(hwnd, NULL);

				BOOL value = TRUE;
				DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));

				resize({ 0 }, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

				break;
			}

			// Removes the default window but keeps the aero shadow
			case WM_NCCALCSIZE:
			{
				if (wpm == TRUE)
				{
					NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lpm);

					sz->rgrc[0].left += GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
					sz->rgrc[0].right -= GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
					sz->rgrc[0].bottom -= GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER);
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

					if (mouse.y < 24 && mouse.x < rgn.right && mouse.x > 0)
					{
						if (mouse.y < GetSystemMetrics(SM_CXFRAME) + GetSystemMetrics(SM_CXPADDEDBORDER)) lRet = HTTOP;
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
	}

	*callDef = _callDef;
	return lRet;
}


LRESULT Panel::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

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