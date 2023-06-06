
#include "panel.h"

Panel::Panel() {
	winName = L"Panel";
}

BOOL Panel::init() {

	defwin::init();
	display(true);

	return 0;
}


LRESULT Panel::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {
	return DefWindowProc(wnd, msg, wpm, lpm);
}
