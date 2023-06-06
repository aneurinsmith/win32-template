
#include "core.h"
#include "win32/window/window.h"


Core::Core() {

}

void Core::save() {

}

BOOL Core::init() {

	_caption = &caption;
	caption.init(_window->hwnd);

	return true;
}

int MARGIN_X = 4;
int MARGIN_Y = 28;

void Core::handle_resize() {
	caption.resize({ 0,1,_window->rgn.right,_window->rgn.top - 1 }, NULL);
}