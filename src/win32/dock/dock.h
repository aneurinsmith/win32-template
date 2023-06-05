
#include "include.h"
#include "wingui.h"
#include "panel/panel.h"

class Dock : public defwin<Dock> {
	friend class Panel;
	
	HWND panels[4];
	Panel panel[4];
	Panel* _panel[4];
	int get_index(POINT);
	POINT get_quadrant(POINT);

public:
	Dock();

	LRESULT init(HWND);
	LRESULT HandleMessage(HWND, UINT, WPARAM, LPARAM);


	void preview(POINT);
	void add(POINT, HWND);
	void remove(HWND);


};
