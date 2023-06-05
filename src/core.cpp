
#include "core.h"
#include "win32/window/window.h"


Core::Core() {
	//params.load();

	string appdata = get_appdata();
	appdata.append("\\test.ini");
	File f;
	f.load(appdata);
}

void Core::save() {
	//params.save(_window->hwnd);
}

LRESULT Core::init() {

	//LOG::console_logger();
	_caption = &caption;
	caption.init(_window->hwnd);

	//_dock = &dock;
	//dock.init(_window->hwnd);

	/*
	_panel1 = &panel1;
	panel1.init(_window->hwnd);
	dock.add({ 0, 0 }, _panel1->hwnd);

	
	_panel2 = &panel2;
	panel2.init(_window->hwnd);
	dock.add({ 3, 10 }, _panel2->hwnd);

	_panel3 = &panel3;
	panel3.init(_window->hwnd);
	dock.add({ 1, 10 }, _panel3->hwnd);
	*/

	return true;
}

int MARGIN_X = 4;
int MARGIN_Y = 28;

void Core::handle_resize() {
	caption.resize({ 0,1,_window->rgn.right,_window->rgn.top-1 }, NULL);
	dock.resize({ 3,104,_window->rgn.right-6, _window->rgn.bottom-127 }, NULL);
}