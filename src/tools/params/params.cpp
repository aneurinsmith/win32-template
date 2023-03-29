
#include "params.h"

bool Params::load() {

	string appdata = get_appdata();
	f.load(appdata.c_str());

	x = f.set("x", NULL) ? f.find("x") : 20;
	y = f.set("y", NULL) ? f.find("y") : 20;
	cx = f.set("cx", NULL) ? f.find("cx") : 800;
	cy = f.set("cy", NULL) ? f.find("cy") : 600;
	fs = f.set("fs", NULL) ? f.find("fs") : 0;

	MONITORINFO mi;
	HMONITOR hMonitor, hMonitor_br, hMonitor_tl;

	hMonitor = MonitorFromPoint({ x + (cx / 2), y + (cy / 2) }, MONITOR_DEFAULTTONEAREST);
	hMonitor_br = MonitorFromPoint({ x + cx, y + cy }, MONITOR_DEFAULTTONEAREST);
	hMonitor_tl = MonitorFromPoint({ x, y }, MONITOR_DEFAULTTONEAREST);

	mi.cbSize = sizeof(mi);
	GetMonitorInfo(hMonitor, &mi);
	RECT window;
	window = mi.rcWork;

	if (x < window.left && hMonitor == hMonitor_tl)
		if (abs(window.left) > abs(x)) x += abs(window.left) - abs(x);
		else x += abs(x) - abs(window.left);
	else if (x + cx > window.right && hMonitor == hMonitor_br)
		x -= x + cx - window.right;
	if (y < window.top && (hMonitor == hMonitor_tl || hMonitor == hMonitor_br))
		if (abs(window.top) > abs(y)) y += abs(window.top) - abs(y);
		else y += abs(y) - abs(window.top);
	else if (y + cy > window.bottom && (hMonitor == hMonitor_tl || hMonitor == hMonitor_br))
		y -= y + cy - window.bottom;

	return 1;
}

void Params::save(HWND wnd) {

	WINDOWPLACEMENT wp;
	GetWindowPlacement(wnd, &wp);

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

RECT Params::get_rect() {
	RECT rc = { x,y,x + cx,y + cy };
	return rc;
}

BOOL Params::get_fs() {
	return fs;
}
