
#pragma once
#include "defwin.h"
#include "tools/button/button.h"

class Caption : public defwin {
	friend class Window;
	friend class Core;

public:
	Caption();
	BOOL init(HWND parent);

protected:
	LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);

	Button close;
	Button maximize;
	Button minimize;

	bool tracking = false;

};
