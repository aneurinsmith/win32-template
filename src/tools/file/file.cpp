
#include "file.h"

File::File(string _path) {
	path = _path;
}

bool File::load() {

	if (path.empty()) return false;

	ifstream stream;
	stream.open(path, ios::in | ios::binary);
	if (!stream.is_open()) return false;

	stream.seekg(0, stream.end);
	int length = stream.tellg();
	stream.seekg(0, stream.beg);

	char* buf = new char[length + 1];
	stream.read(buf, length);
	stream.close();
	buf[length] = 0;

	int count;
	if (length > 0) count = 1;
	else count = 0;
	char* p = buf;
	while (*p) {
		if (*p == '\n') count++;
		*p++;
	}
	p = buf;

	for (int i = 0; i < count; i++) {
		string line;
		while (*p) {
			if (*p == '\r' || *p == '\n') {
				*p++;
				if (*p == '\r' || *p == '\n') {
					*p++;
					break;
				}
			}
			line.append(p, 1);
			*p++;
		}
		lines.push_back(line);
	}

	return true;
}

bool File::load(string _path) {
	path = _path;
	return load();
}

bool File::save() {


	if (path.empty()) return false;
	string temp_path = path;
	temp_path.resize(temp_path.find_last_of("\\"));
	CreateDirectoryA(temp_path.c_str(), NULL);

	ofstream stream;
	stream.open(path, ios::out);
	if (!stream.is_open()) return false;

	string buf = get_buffer();
	stream.write(buf.c_str(), buf.length());

	stream.close();

	return true;
}

bool File::save(string _path) {
	path = _path;
	return save();
}

bool File::write(string text) {
	lines.push_back(text);

	return 0;
}

bool File::write(string text, int index) {
	lines[index] = text;

	return 0;
}


int File::get_index(string text) {
	return 0;
}

string File::get_line(int index) {
	string str("123");
	return str;
}

string File::get_buffer() {
	string buffer;
	for (int i = 0; i < lines.size(); i++) {
		buffer.append(lines[i]);
		if(i != lines.size()-1) buffer.append("\n");
	}
	return buffer;
}

string File::get_appdata() {
	string path;

	char appdata[MAX_PATH];
	SHGetSpecialFolderPathA(NULL, appdata, CSIDL_APPDATA, 0);

	path.append(appdata);
	path.append("\\Template");
	return path;
}
