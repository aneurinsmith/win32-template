
#pragma once
#include "include.h"

class File {

public:

	File();
	~File();

	virtual File& open(string _name, int _mode);
	File& open(string _path, string _name, int _mode);
	File& open(int _path, string _name, int _mode);

	virtual bool flush();
	bool close();

	virtual vector<string> sync();
	vector<string> read();

	int write(string _text);
	int write(string _text, int _index);
	int insert(string _text, int _index);

	int tell();
	int seek(int _index, int _way = ios::beg);

protected:

	string name;
	int mode;
	int lineHead;
	int charHead;

	ifstream istream;
	ofstream ostream;
	vector<string> sbuffer;

	mutex m;
	unique_lock<mutex> lk;

};
