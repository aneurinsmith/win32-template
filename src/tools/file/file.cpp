
#include "file.h"

File::File() {
	unique_lock<mutex> tmp(m, defer_lock);
	lk.swap(tmp);
}

File::~File() {
	if (lk.owns_lock()) lk.unlock();
}

File& File::open(string _name, int _mode) {
	lk.lock();

	name = _name;
	mode = _mode;
	sync();

	return *this;
}
File& File::open(string _path, string _name, int _mode) {
	string path;

	path += _path;
	path += _name;

	return open(path, _mode);
}
File& File::open(int _path, string _name, int _mode) {
	string path;

	char address[MAX_PATH];
	SHGetSpecialFolderPathA(NULL, address, _path, 0);
	path += address;
	path += "\\";
	path += _name.substr(0, _name.rfind("\\"));
	CreateDirectoryA(path.c_str(), NULL);
	path += _name.substr(_name.rfind("\\"), _name.size());

	return open(path, _mode);
}



bool File::flush() {
	if (mode & ios::out) {
		ostream.open(name, mode & ios::trunc);

		vector<string> obuffer = sbuffer;
		if (mode & ios::app) sbuffer.clear();

		if (mode & ios::binary) {
			ostream.write(obuffer[0].c_str(), obuffer[0].size());
			seek(sbuffer[0].size());
		}
		else {
			for (int i = 0; i < sbuffer.size(); i++) {
				obuffer[i] += '\n';
				ostream.write(obuffer[i].c_str(), obuffer[i].size());
			}
			seek(sbuffer.size());
		}
		ostream.flush();
		ostream.close();
		return ostream.good();
	}
	else {
		return false;
	}
}
bool File::close() {
	if (lk.owns_lock()) lk.unlock();
	bool res = flush();
	sbuffer.clear();
	return res;
}



vector<string> File::sync() {
	vector<string> _sbuffer;

	if (mode & ios::in && !(mode & ios::app) && !(mode & ios::trunc)) {
		istream.open(name, mode);
		istream.seekg(0);
		istream.sync();
		if (mode & ios::binary) {
			string sbuf;

			for (char c; istream.get(c);) {
				sbuf += c;
			}
			_sbuffer.push_back(sbuf);

			if (mode & ios::ate) seek(_sbuffer[0].size());
		}
		else {

			for (string line; getline(istream, line);) {
				_sbuffer.push_back(line);
			}
			if (mode & ios::ate) seek(_sbuffer.size());
		}
		istream.close();
	}

	sbuffer = _sbuffer;

	return _sbuffer;
}
vector<string> File::read() {
	return sbuffer;
}



int File::write(string _text) {
	if (mode & ios::out) {
		return write(_text, (mode & ios::binary ? charHead : lineHead));
	}
	else {
		return -1;
	}
}
int File::write(string _text, int _index) {
	if (mode & ios::out) {
		if (mode & ios::binary) {
			
			if (_index >= sbuffer[0].size()) {
				sbuffer[0].append(_text);
			}
			else {
				sbuffer[0].replace(_index, _text.size(), _text);
			}

			charHead += _text.size();
			return _text.size();
		}
		else {

			if (_index >= sbuffer.size()) {
				sbuffer.push_back(_text);
			}
			else {
				sbuffer[_index] = _text;
			}

			charHead += _text.size();
			lineHead++;
			return _text.size();
		}
	}
	else {
		return -1;
	}
}
int File::insert(string _text, int _index) {
	if (mode & ios::out) {
		if (mode & ios::binary) {

			// BINARY
			// _index is char

		}
		else {

			// NON BINARY
			// _index is line

		}
	}
	else {
		return -1;
	}
}



int File::tell() {
	return lineHead;
	//return head;
}
int File::seek(int _index, int _way) {
	istream.clear();
	ostream.clear();
	if (mode & ios::binary) {

		if (_way != ios::cur) {
			charHead = 0;

			if (_way == ios::end) {
				charHead = sbuffer.size();
			}
		}

		charHead += _index;
		return charHead;
	}
	else {

		int index = 0, way = 0;

		if (_way != ios::cur) {
			charHead = 0;
			lineHead = 0;
			if (_way == ios::end) {
				for (int i = 0; i < sbuffer.size(); i++) {
					charHead += sbuffer[i].size();
					lineHead++;
				}
			}
		}

		for (int i = 0; i < sbuffer.size() && i < _index; i++) {
			charHead += sbuffer[i].size();
			lineHead++;
		}

		return charHead;
	}

	return -1;
}