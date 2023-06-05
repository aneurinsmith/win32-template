#pragma once
#include "include.h"
#include "tools/file/file.h"
//#include "tools/params/params.h"

#include "win32/dock/dock.h"
#include "win32/panel/panel.h"
#include "win32/caption/caption.h"



class Window;
class Panel;
class Caption;

class Core final {
	friend class Window;
public:

	Core();

	void save();

	LRESULT init();

private:
	//Params params;

	Window* _window = nullptr;

	Caption caption;
	Caption* _caption = nullptr;

	Dock dock;
	Dock* _dock = nullptr;

	Panel panel1;
	Panel* _panel1 = nullptr;

	Panel panel2;
	Panel* _panel2 = nullptr;

	Panel panel3;
	Panel* _panel3 = nullptr;


protected:
	void handle_resize();
};

