
#include "canvas.h"

Canvas::Canvas() {
	winName = L"Canvas";
}

BOOL Canvas::init() {

	defwin::init();
	display(true);

	return 0;
}


LRESULT Canvas::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {
	return DefWindowProc(wnd, msg, wpm, lpm);
}
