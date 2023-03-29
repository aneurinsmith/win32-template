#pragma once
#include "include.h"
#include "tools/file/file.h"

class Params {
private:
	File f;
	LONG x = -1, 
		 y = -1,
		 cx = -1,
		 cy = -1;
	BOOL fs = -1;

public:
	bool load();
	void save(HWND);

	RECT get_rect();
	BOOL get_fs();
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
