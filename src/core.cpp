
#include "core.h"
#include "win32/window/window.h"

Core::Core() {
	params.load();
}

void Core::save() {
	params.save(_window->hwnd);
}

LRESULT Core::init() {
	_caption = &caption;
	caption.init(_window->hwnd);

	_panel1 = &panel1;
	panel1.init(_window->hwnd);

	_panel2 = &panel2;
	panel2.init(_window->hwnd);

	return true;
}

int MARGIN_X = 4;
int MARGIN_Y = 28;

void Core::handle_resize() {
	caption.resize({ 0,1,_window->rgn.right,_window->rgn.top-1 }, NULL);

	if (panel1.docked) {
		panel1.resize({
			MARGIN_X,
			(_window->rgn.top) + MARGIN_X,
			(_window->rgn.right / 2) - MARGIN_X,
			(_window->rgn.bottom) - (_window->rgn.top) - MARGIN_Y
		}, NULL);
	}

	if (panel2.docked) {
		panel2.resize({
			(_window->rgn.right / 2) + MARGIN_X,
			(_window->rgn.top) + MARGIN_X,
			(_window->rgn.right) - (_window->rgn.right / 2) - (MARGIN_X * 2),
			(_window->rgn.bottom) - (_window->rgn.top) - MARGIN_Y
		}, NULL);
	}
}