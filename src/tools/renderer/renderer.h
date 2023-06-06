
#pragma once
#include "include.h"

class Renderer {

public:
	void attach(HWND wnd);

	void begin_draw();
	void end_draw();

	void resize_buffer(RECT region);
	void invalidate_rect(RECT _rc);

	void draw_rect(RECT _rc, COLORREF _br);
	void draw_icon(RECT _rc, HICON _i);

private:
	HWND assoc_wnd;
	RECT rgn;

	PAINTSTRUCT ps;
	HDC dc, memdc;
	HPAINTBUFFER hbuffer;

	BP_PAINTPARAMS params = {
		sizeof(params),
		BPPF_NOCLIP | BPPF_ERASE
	};

};
