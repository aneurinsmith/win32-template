#pragma once
#include "include.h"
#include "tools/file/file.h"
#include "panel/panel.h"
#include "caption/caption.h"

struct Parameters {
	bool fs = -1;
	LONG x = -1;
	LONG y = -1;
	LONG cx = -1;
	LONG cy = -1;
};

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
	File f;
	Parameters params;
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

inline string get_appdata() {
	string p;
	char appdata[MAX_PATH];

	SHGetSpecialFolderPathA(NULL, appdata, CSIDL_APPDATA, 0);

	p.append(appdata);
	p.append("\\Template");
	_mkdir(p.c_str());
	p.append("\\template.ini");
	return p;
}