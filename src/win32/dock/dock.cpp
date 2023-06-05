
#include "dock.h"

Dock::Dock() {
	winClass = L"dock";
	winName = L"dock";
	style = WS_CHILD;
}

LRESULT Dock::init(HWND parent) {
	prnt = parent;
	defwin::init();

	SetWindowLong(hwnd, GWL_STYLE, 0);

	_panel[0] = &panel[0];
	panel[0].init(hwnd);
	add({0,0}, _panel[0]->hwnd);

	resize({ 0 }, SWP_NOSIZE | SWP_NOMOVE | SWP_FRAMECHANGED);
	display(true);

	return true;
}


LRESULT Dock::HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

	switch (msg) 
	{
		case WM_CREATE: 
		{

			break;
		}

		case WM_NCHITTEST: {
			return HTTRANSPARENT;
		}

		case WM_MOUSEMOVE :
		{
			POINT p = get_quadrant({ GET_X_LPARAM(lpm),  GET_Y_LPARAM(lpm) });

			cout << p.x << p.y << endl;
			break;
		}

		case WM_SIZE: 
		{
			if (panels[0]) {
				if (!panels[1] && !panels[2] && !panels[3]) {
					SetWindowPos(panels[0], nullptr, pos.x, pos.y, rgn.right, rgn.bottom, NULL);
					break;
				}
				else if (!panels[1]) {
					SetWindowPos(panels[0], nullptr, pos.x, pos.y, rgn.right - 2, rgn.bottom / 2 - 2, NULL);
				}
				else {
					SetWindowPos(panels[0], nullptr, pos.x, pos.y, rgn.right / 2 - 2, rgn.bottom / 2 - 2, NULL);
				}
			}

			if (panels[3]) {
				if (!panels[2]) {
					SetWindowPos(panels[3], nullptr, pos.x, pos.y + rgn.bottom / 2 + 2, rgn.right, rgn.bottom / 2 - 2, NULL);
				}
				else {
					SetWindowPos(panels[3], nullptr, pos.x + rgn.right / 2 + 2, pos.y + rgn.bottom / 2 + 2, rgn.right / 2 - 2, rgn.bottom / 2 - 2, NULL);
				}
			}
			if (panels[2]) {
				if (!panels[0]) {
					SetWindowPos(panels[2], nullptr, pos.x, pos.y, rgn.right / 2 - 2, rgn.bottom, NULL);
				}
				else {
					SetWindowPos(panels[2], nullptr, pos.x, pos.y + rgn.bottom / 2 + 2, rgn.right / 2 - 2, rgn.bottom / 2 - 2, NULL);
				}
			}
			if (panels[1]) {
				if (!panels[3]) {
					SetWindowPos(panels[1], nullptr, pos.x + rgn.right / 2 + 2, pos.y, rgn.right / 2 - 2, rgn.bottom, NULL);
				}
				else {
					SetWindowPos(panels[1], nullptr, pos.x + rgn.right / 2 + 2, pos.y, rgn.right / 2 - 2, rgn.bottom / 2 - 2, NULL);
				}
			}
			break;
		}


		default: 
		{
			return DefWindowProc(wnd, msg, wpm, lpm);
		}
	}
	return 0;
}

/*

	###################  ###################  ###################  ###################  ###################  ###################  ###################  ###################
	##       #       ##  ##       #       ##  ##               ##  ##       #       ##  ##       #       ##	 ##       #       ##  ##               ##  ##               ##
	##   0   #   1   ##  ##       #   1   ##  ##       0       ##  ##   0   #       ##  ##   0   #   1   ##	 ##       #       ##  ##       0       ##  ##               ##
	##       #       ##  ##       #       ##  ##               ##  ##       #       ##  ##       #       ##	 ##       #       ##  ##               ##  ##               ##
	###################  ##   2   ##########  ###################  ##########   1   ##  ###################  ##   2   #   1   ##  ###################  ##       0       ##
	##       #       ##  ##       #       ##  ##       #       ##  ##       #       ##  ##               ##	 ##       #       ##  ##               ##  ##               ##
	##   2   #   3   ##  ##       #   3   ##  ##   2   #   3   ##  ##   2   #       ##  ##       3       ##	 ##       #       ##  ##       3       ##  ##               ##
	##       #       ##  ##       #       ##  ##       #       ##  ##       #       ##  ##               ##	 ##       #       ##  ##               ##  ##               ##
	###################  ###################  ###################  ###################  ###################  ###################  ###################  ###################

	LOGIC:
	-- index 0
		-- main if all null
		-- top if 1 null
		-- top left if 1 not null

	-- index 1
		-- right if 3 null
		-- top right if 3 not null

	-- index 2
		-- left if 0 null
		-- bottom left if 0 not null

	-- index 3
		-- bottom if 2 null
		-- bottom right if 2 not null

*/

POINT Dock::get_quadrant(POINT mouse) {
	POINT quadrant = { 0,0 };

	if (mouse.x > rgn.right / 2) {
		quadrant.x = 1;
	}

	if (mouse.y > rgn.bottom / 2) {
		quadrant.y = 1;
	}

	return quadrant;
}

int Dock::get_index(POINT mouse) {
	int index = 1;

	if(mouse.x >= rgn.right/2) {
		index = 1;
	}
	else if (mouse.x < rgn.right / 2) {
		index = 0;
	}

	else {
		return 0;
	}








	#ifdef _DEBUG
		cout << "dock index = " << index << endl;
	#endif

	return index;
}



void Dock::add(POINT mouse, HWND panel) {
	panels[mouse.x] = panel;
}

void Dock::remove(HWND panel) {

}
