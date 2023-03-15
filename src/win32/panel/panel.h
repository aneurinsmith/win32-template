#pragma once
#include "include.h"
#include "wingui.h"

class Panel : public defwin<Panel> {
	friend class Window;
	friend class Core;
public:
	Panel();

	LRESULT init(HWND);

	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);

};