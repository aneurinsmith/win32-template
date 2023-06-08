
#pragma once
#include "include.h"

class File {

public:
	File() = default;
	File(string _path);

	bool load();
	bool load(string _path);
	bool save();
	bool save(string _path);
	bool write(string _text);
	bool write(string _text, int index);

	int get_index(string _text);
	string get_line(int index);

	string get_buffer();

	static string get_appdata();

private:

	string path;
	vector<string> lines;

};
