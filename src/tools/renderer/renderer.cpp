
#include "renderer.h"

void Renderer::attach(HWND wnd) {
	assoc_wnd = wnd;
}

void Renderer::begin_draw() {
	BufferedPaintInit();
	dc = BeginPaint(assoc_wnd, &ps);
	hbuffer = BeginBufferedPaint(dc, &rgn, BPBF_TOPDOWNDIB, NULL, &memdc);
}

void Renderer::end_draw() {
	BufferedPaintSetAlpha(hbuffer, &rgn, 255);
	EndBufferedPaint(hbuffer, TRUE);
	EndPaint(assoc_wnd, &ps);
	BufferedPaintUnInit();
}

void Renderer::resize_buffer(RECT region) {
	rgn = region;
}

void Renderer::invalidate_rect(RECT _rc) {
	InvalidateRect(assoc_wnd, &_rc, false);
}

void Renderer::draw_rect(RECT _rc, COLORREF _br) {
	HBRUSH br = CreateSolidBrush(_br);
	RECT rc = _rc;
	FillRect(memdc, &rc, br);
	DeleteObject(br);
}

void Renderer::draw_icon(RECT _rc, HICON _i) {
	DrawIconEx(memdc, _rc.left - (_rc.right / 2), _rc.top - (_rc.bottom / 2), _i, _rc.right, _rc.bottom,
		0, NULL, DI_NORMAL | DI_COMPAT);
}