
#pragma once
#include "include.h"
#include "wingui.h"
#include "tools/file/file.h"

class Panel : public defwin<Panel> {
	//friend class Window;
	//friend class Core;
	friend class Dock;

	bool docked = true;
public:

	Panel();

	LRESULT init(HWND);

	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);
	LRESULT HandleDef(HWND, UINT, WPARAM, LPARAM);
	LRESULT HandleDWM(HWND, UINT, WPARAM, LPARAM, BOOL*);

};