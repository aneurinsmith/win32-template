
#include "panel.h"

Panel::Panel() {
	winClass = L"panel";
	winName = L"panel";
	style = WS_CHILD;
	size = { 0,0 };
}

LRESULT Panel::init(HWND parent) {

	prnt = parent;
	defwin::init();

	SetWindowLong(hwnd, GWL_STYLE, 0);

	resize({ 0 }, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
	display(true);

	return true;
}

LRESULT Panel::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

	switch (msg)
	{
		case WM_NCHITTEST: 
		{
			return HTTRANSPARENT;
		}

		case WM_PAINT: 
		{
			r.begin_draw();
			r.draw_rect(rgn, 0x212021);
			r.end_draw();
			
			break;
		}

		case WM_DESTROY:
		{
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