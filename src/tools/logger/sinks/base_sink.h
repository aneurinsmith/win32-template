/*
	\brief		The interface to be used by each sink
	\file		base_sink.h
	\author		Aneurin F. Smith
*/

#include "include.h"

enum Level:int;
class basesink {

public:
	~basesink() = default;
	virtual void set_level(const Level) = 0;
	virtual void print(const Level, const string) = 0;

};
