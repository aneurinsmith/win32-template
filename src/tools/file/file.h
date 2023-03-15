
#include "include.h"

/*
*	\brief		To-Do
*	\author		Aneurin Smith
*/
struct Row {
	int length;
	string _item;
	string _val;
	string _buffer() {
		string merge;

		merge.append(_item);
		if(_val.length() > 0)merge.append("=");
		merge.append(_val);

		return merge;
	}
};

/*
*	\brief		To-Do
*	\author		Aneurin Smith
*/
class File {

private:
	FILE* _stream = 0;
	string path;
	Row* rows;

public:
	File();

	/*
	*	\brief		Loads a system file
	*	\param		path – the string containing the file path to be loaded
	*	\see
	*/
	bool load(string path);

	/*
	*	\brief		To-Do
	*	\param		param
	*/
	int find(string item);

	/*
	*	\brief		The function assigns val to the row item if found. Also works as isSet().
	*	\param		item – The value to search for.
	*	\param		val – The value to assign to the item if found. If the value is NULL, dont set the value.
	*	\returns	True if the item exists, false if not.
	*	\see s
	*/

	bool set(string item, const char* val);

	/*
	*	\brief		To-Do
	*/
	bool save();

};
