
#pragma once
#include "defwin.h"
#include "core/core.h"

class Window : public defwin {
	friend class Core;

public:
	Window();
	BOOL init();

protected:
	LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	LRESULT HandleDef(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);
	LRESULT HandleDWM(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm, BOOL*);

private:
	Core core;
};
