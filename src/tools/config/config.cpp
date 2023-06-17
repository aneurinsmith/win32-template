
#include "config.h"

File& Config::open(string _name, int _mode) {
	_mode &= ~ios::binary;
	File::open(_name, _mode);
	return *this;
}

bool Config::close() {
	bool res = File::close();
	sec.clear();
	return res;
}



vector<string> Config::sync() {
	vector<string> _sbuffer = File::sync();

	string head = "", key, val;
	for (auto line : sbuffer) {
		if (line.substr(0, 1) == "[" && line.substr(line.size() - 1, 1) == "]") {
			head = line.substr(1, line.size() - 2);
		}
		else {
			try {
				key = line.substr(0, line.find("="));
				val = line.substr(line.find("=") + 1, line.size());
				sec[head][key] = stoi(val);
			}
			catch (...) {
				continue;
			}
		}
	}

	return _sbuffer;
}



bool Config::flush() {

	vector<string> lines;

	for (auto section : sec) {
		lines.push_back("[" + section.first + "]");
		for (auto key : section.second) {
			lines.push_back(key.first + "=" + to_string(key.second));
		}
		lines.push_back("");
	}
	sbuffer = lines;
	return File::flush();
}



bool Config::isSet(string _sec, string _key) {
	return sec.find(_sec) != sec.end() && sec[_sec].find(_key) != sec[_sec].end();
}
int Config::getVal(string _sec, string _key) {
	if (mode & ios::in && isSet(_sec, _key)) {
		return sec[_sec][_key];
	}
	else {
		return -1;
	}
}
void Config::setVal(string _sec, string _key, int _val) {
	if (mode & ios::out) {
		sec[_sec][_key] = _val;
	}
}