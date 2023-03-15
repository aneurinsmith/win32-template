
#include "include.h"
#include "wingui.h"

class Button {
protected:
	RECT rect;
	SIZE size;
	INT state = 0;

public:
	HICON icon;
	SIZE iSize = { 16,16 };
	COLORREF cols [3] = { COL_HEAD, COL_HEAD+0x101010, COL_HEAD+0x202020 };

	Button() = default;

	Button(RECT _r);
	Button(RECT _r, HICON _i);

	void move(POINT _p);
	void draw(renderer _renderer);
	bool contains(POINT _p);

	void set_state(renderer _renderer, INT _state);
	
};