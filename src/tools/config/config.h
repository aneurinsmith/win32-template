
#pragma once
#include "include.h"

#include "tools/file/file.h"

class Config : File {

public:
	Config() = default;

	using File::open;

	File& open(string _name, int _mode);
	bool close();
	vector<string> sync();
	bool flush();

	bool isSet(string _sec, string _key);
	int getVal(string _sec, string _key);
	void setVal(string _sec, string _key, int _val);


private:
	map<string, map<string, int>> sec;

};
