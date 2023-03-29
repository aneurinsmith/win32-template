
#include "button.h"


Button::Button(RECT _r) {
	rect = _r;
	size = { _r.right - _r.left, _r.bottom - _r.top };
}
Button::Button(RECT _r, HICON _i) {
	rect = _r;
	size = { _r.right - _r.left, _r.bottom - _r.top };
	icon = _i;
}

void Button::move(POINT _p) {
	rect.left = _p.x;
	rect.top = _p.y;
	rect.right = _p.x + size.cx;
	rect.bottom = _p.y + size.cy;
}

void Button::draw(Renderer _renderer) {
	_renderer.draw_rect(rect, cols[state]);
	_renderer.draw_icon({ (rect.left + (rect.right - rect.left) /2), (rect.top + (rect.bottom - rect.top) / 2),iSize.cx,iSize.cy }, icon);
}

bool Button::contains(POINT _p) {
	if (_p.x >= rect.left && _p.y >= rect.top && _p.x <= rect.right && _p.y <= rect.bottom) {
		return true;
	}
	else {
		return false;
	}
}

void Button::set_state(Renderer _renderer, INT _state) {
	if (state != _state) {
		_renderer.invalidate_rect(rect);
	}
	state = _state;
}