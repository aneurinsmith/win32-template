
#pragma once
#include "panel/panel.h"

class Canvas : public Panel {

public:
	Canvas();
	BOOL init();

protected:
	LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);

};
