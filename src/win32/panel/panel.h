#pragma once
#include "include.h"
#include "wingui.h"

class Panel : public defwin<Panel> {
	friend class Window;
	friend class Core;

	bool docked = true;
public:

	Panel();

	LRESULT init(HWND);

	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);
	LRESULT HandleDef(HWND, UINT, WPARAM, LPARAM);
	LRESULT HandleDWM(HWND, UINT, WPARAM, LPARAM, BOOL*);

};