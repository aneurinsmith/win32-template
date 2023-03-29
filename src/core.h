#pragma once
#include "include.h"
#include "tools/dock/dock.h"
#include "tools/params/params.h"

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
	Params params;

	Window* _window = nullptr;

	Caption caption;
	Caption* _caption = nullptr;

	Panel panel1;
	Panel* _panel1 = nullptr;

	Panel panel2;
	Panel* _panel2 = nullptr;


protected:
	void handle_resize();
};
