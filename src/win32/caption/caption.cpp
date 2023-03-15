
#include "caption.h"

Caption::Caption() {
	winClass = L"caption";
	winName = L"caption";
	style = WS_CHILD;
	size = { 0,0 };
}


LRESULT Caption::init(HWND parent) {

	prnt = parent;
	defwin::init();

	SetWindowLong(hwnd, GWL_STYLE, 0);

	resize({ 0 }, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
	display(true);

	return true;
}


LRESULT Caption::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

	switch (msg)
	{
		case WM_CREATE: {

			close = Button({ 0,0,45,29 }, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_CLOSE)));
			maximize = Button({ 0,0,45,29 }, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAXIMIZE)));
			minimize = Button({ 0,0,45,29 }, LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MINIMIZE)));

			BOOL t = TRUE;
			DwmSetWindowAttribute(wnd, DWMWA_CLOAK, &t, sizeof(t));

			break;
		}

		case WM_SIZE: {

			if (wpm == SIZE_MAXIMIZED) {
				maximize.icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_RESTORE));
				rgn.top = 7;
			}
			else {
				maximize.icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAXIMIZE));
				rgn.top = 0;
			}

			close.move({ GET_X_LPARAM(lpm) - 45, rgn.top });
			maximize.move({ GET_X_LPARAM(lpm) - 91, rgn.top });
			minimize.move({ GET_X_LPARAM(lpm) - 137, rgn.top });

			break;
		}

		case WM_MOUSEACTIVATE: {
			return MA_NOACTIVATE;
		}

		case WM_MOUSEMOVE: {
			if (!tracking) {
				TRACKMOUSEEVENT tme = {};
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = wnd;
				tme.dwFlags = TME_LEAVE;

				if(_TrackMouseEvent(&tme)) tracking = true;
			}
			break;
		}

		case WM_MOUSELEAVE: {
			close.set_state(r, normal);
			maximize.set_state(r, normal);
			minimize.set_state(r, normal);
			tracking = false;
			break;
		}

		case WM_LBUTTONDOWN: {
			POINT mouse = { GET_X_LPARAM(lpm), GET_Y_LPARAM(lpm) };

			if (close.contains(mouse)) close.set_state(r, click);
			if (maximize.contains(mouse)) maximize.set_state(r, click);
			if (minimize.contains(mouse)) minimize.set_state(r, click);

			break;
		}

		case WM_LBUTTONUP: {
			POINT mouse = { GET_X_LPARAM(lpm), GET_Y_LPARAM(lpm) };

			if (close.contains(mouse)) {
				SendMessage(prnt, WM_DESTROY, 0, 0);
			}

			if (maximize.contains(mouse)) {
				WINDOWPLACEMENT wp;
				GetWindowPlacement(prnt, &wp);

				if (!wp.flags) {
					SendMessage(prnt, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				}
				else {
					SendMessage(prnt, WM_SYSCOMMAND, SC_RESTORE, 0);
				}
			}

			if (minimize.contains(mouse)) {
				SendMessage(prnt, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}

			break;
		}

		case WM_NCHITTEST:
		{
			LRESULT lRet = HTTRANSPARENT;

			POINT mouse = { GET_X_LPARAM(lpm), GET_Y_LPARAM(lpm) };
			ScreenToClient(hwnd, &mouse);

			if (close.contains(mouse)) {
				close.set_state(r, hover);
				lRet = HTCLIENT;
			}
			else {
				close.set_state(r, normal);
			}

			if (maximize.contains(mouse)) {
				maximize.set_state(r, hover);
				lRet = HTCLIENT;
			}
			else {
				maximize.set_state(r, normal);
			}

			if (minimize.contains(mouse)) {
				minimize.set_state(r, hover);
				lRet = HTCLIENT;
			}
			else {
				minimize.set_state(r, normal);
			}

			return lRet;
		}

		case WM_PAINT:
		{
			r.begin_draw();
			r.draw_rect(rgn, COL_HEAD);

			close.draw(r);
			maximize.draw(r);
			minimize.draw(r);

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