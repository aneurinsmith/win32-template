
#include "file.h"

File::File() {

}

bool File::load(string _path) {
	path = _path;
	rows = new Row[0];
	rows->length = 0;

	errno_t err = fopen_s(&_stream, path.c_str(), "rb");
	if (err != 0) return false;

	int length = 0;
	fseek(_stream, 0, SEEK_END);
	length = ftell(_stream);
	fseek(_stream, 0, SEEK_SET);

	char* buf = new char[length + 1];
	buf[0] = 0;
	if(fread(buf, length, 1, _stream) != 1) return false;
	buf[length] = 0;

	string item;
	string val;

	char* p = buf;
	bool before = true;
	int lcols = 0;
	int rcols = 0;

	int count = 0;
	while (*p) {
		if (*p == '\n') count++;
		*p++;
	}
	p = buf;
	rows = new Row[count + 1];
	rows->length = count+1;

	for (int i = 0; i < rows->length; i++) {
		while (*p) {

			if (*p == '=') {
				before = false;
			}
			else if (*p == '\r' || *p == '\n') {
				*p++;
				if (*p == '\r' || *p == '\n') *p++;

				before = true;
				break;
			}
			else {
				before ? item.append(p, 1) : val.append(p, 1);
			}
			*p++;
		}
		rows[i]._item = item;
		rows[i]._val = val;
		item.clear();
		val.clear();
	}
	fclose(_stream);
	return true;
}

bool File::save() {
	if (!rows) return false;
	errno_t err = fopen_s(&_stream, path.c_str(), "wb");
	if (err != 0) return false;

	for (int i = 0; i < rows->length; i++) {
		fprintf(_stream, "%s", rows[i]._buffer().c_str());
		if(i != rows->length - 1) fprintf(_stream, "\r\n");
	}

	fclose(_stream);
	return true;
}

int File::find(string item) {

	for (int i = 0; i < rows->length; i++) {
		if (rows[i]._item == item) {
			return stoi(rows[i]._val);
		}
	}
	return -1;
}

bool File::set(string item, const char* val) {

	for (int i = 0; i < rows->length; i++) {
		if (rows[i]._item == item) {
			if (val) {
				rows[i]._val = val;
			}
			return true;
		}
	}

	if (val) {
		Row* p;
		p = new Row[rows->length];
		p->length = rows->length;
		for (int i = 0; i < rows->length; i++) {
			*(p + i) = rows[i];
		}

		rows = new Row[p->length + 1];
		copy(p, p + p->length, rows);
		rows->length = p->length + 1;

		rows[p->length]._item = item;
		rows[p->length]._val = val;

	}

	return false;
}