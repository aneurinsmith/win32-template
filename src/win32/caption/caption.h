#pragma once
#include "include.h"
#include "wingui.h"
#include "button/button.h"


enum State {normal,hover,click};

class Core;

class Caption : public defwin<Caption> {
friend class Window;
friend class Core;
public:
	Caption();

	LRESULT init(HWND);

	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);

	Button close;
	Button maximize;
	Button minimize;

	bool tracking = false;

};