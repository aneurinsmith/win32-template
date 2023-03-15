
#include "core.h"
#include "win32/window/window.h"

Core::Core() {

	string appdata = get_appdata();
	f.load(appdata.c_str());

	params.x = f.set("x", NULL) ? f.find("x") : 20;
	params.y = f.set("y", NULL) ? f.find("y") : 20;
	params.cx = f.set("cx", NULL) ? f.find("cx") : 800;
	params.cy = f.set("cy", NULL) ? f.find("cy") : 600;
	params.fs = f.set("fs", NULL) ? f.find("fs") : 0;

	MONITORINFO mi;
	HMONITOR hMonitor, hMonitor_br, hMonitor_tl;

	hMonitor = MonitorFromPoint({params.x + (params.cx/2), params.y + (params.cy/2)}, MONITOR_DEFAULTTONEAREST);
	hMonitor_br = MonitorFromPoint({ params.x + params.cx, params.y + params.cy }, MONITOR_DEFAULTTONEAREST);
	hMonitor_tl = MonitorFromPoint({ params.x, params.y }, MONITOR_DEFAULTTONEAREST);

	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);
	RECT window;
	window = mi.rcWork;

	if (params.x < window.left && hMonitor == hMonitor_tl)
		if (abs(window.left) > abs(params.x)) params.x += abs(window.left) - abs(params.x);
		else params.x += abs(params.x) - abs(window.left);
	else if (params.x + params.cx > window.right && hMonitor == hMonitor_br)
		params.x -= params.x + params.cx - window.right;
	if (params.y < window.top && (hMonitor == hMonitor_tl || hMonitor == hMonitor_br))
		if (abs(window.top) > abs(params.y)) params.y += abs(window.top) - abs(params.y);
		else params.y += abs(params.y) - abs(window.top);
	else if (params.y + params.cy > window.bottom && (hMonitor == hMonitor_tl || hMonitor == hMonitor_br))
		params.y -= params.y + params.cy - window.bottom;
}

void Core::save() {
	WINDOWPLACEMENT wp;
	GetWindowPlacement(_window->hwnd, &wp);

	f.set("[General]", "");
	f.set("", "");
	f.set("[Window]", "");
	f.set("x", to_string(wp.rcNormalPosition.left).c_str());
	f.set("y", to_string(wp.rcNormalPosition.top).c_str());
	f.set("cx", to_string(wp.rcNormalPosition.right - wp.rcNormalPosition.left).c_str());
	f.set("cy", to_string(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top).c_str());
	f.set("fs", to_string(wp.flags == 2 ? 1 : 0).c_str());

	f.save();
}

LRESULT Core::init(HWND hwnd) {
	_caption = &caption;
	caption.init(_window->hwnd);

	_panel1 = &panel1;
	panel1.init(_window->hwnd);

	_panel2 = &panel2;
	panel2.init(_window->hwnd);

	return true;
}

void Core::handle_resize() {
	caption.resize({ 0,1,_window->rgn.right,_window->rgn.top-1 }, NULL);

	panel1.resize({ 
		4,
		_window->rgn.top + 4, 
		400,
		_window->rgn.bottom - _window->rgn.top - 26
		}, NULL);

	panel2.resize({
		408,
		_window->rgn.top + 4,
		_window->rgn.right - 412,
		_window->rgn.bottom - _window->rgn.top - 26
		}, NULL);

}