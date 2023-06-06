
#pragma once
#include "include.h"
#include "caption/caption.h"

class Window;
class Caption;

class Core final {
	friend class Window;
public:

	Core();

	void save();

	BOOL init();

private:
	Window* _window = nullptr;

	Caption caption;
	Caption* _caption = nullptr;

protected:
	void handle_resize();
};

