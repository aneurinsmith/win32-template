#pragma once
#include "include.h"
#include "wingui.h"
#include "core.h"
#include "tools/bitmap/bitmap.h"

class Window : public defwin<Window> {
friend class Core;

protected:
	void attach_wnd();

public:
	static int sys_height;
	static int sys_frame;
	Window();

	LRESULT init();

	//static LRESULT CALLBACK MainProc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm);

	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);
	LRESULT HandleDef(HWND, UINT, WPARAM, LPARAM);
	LRESULT HandleDWM(HWND, UINT, WPARAM, LPARAM, BOOL*);
	LRESULT HandleDarkMode(HWND, UINT, WPARAM, LPARAM);

private:
	Core core;

};