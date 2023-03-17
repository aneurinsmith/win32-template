/*
	\brief		Declares the File class, and the Row structure used by the File class.
	\file		file.h
	\author		Aneurin F. Smith
*/

#include "include.h"

/*
	\brief		The structure used to store row information.
	\see		https://github.com/AneurinSmith/win32-template/wiki/file#row
*/
struct Row {
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
	\brief		A file interface, intended for loading and saving application data to allow for persistence. See documentation for full usecases.
	\see		https://github.com/AneurinSmith/win32-template/wiki/file#file
*/
class File {

private:
	FILE* _stream = 0;
	string path;
	Row* rows;
	int total_rows;

public:

	/*
		\brief		Loads a system file from the provided path
		\param			path – A handle to the current instance of the application.
		\returns		True if file is loaded correctly, false if loading fails.
		\see		https://github.com/AneurinSmith/win32-template/wiki/file#load
	*/
	bool load(string path);

	/*
		\brief		Finds the value assigned to the requested item.
		\param			item – The item name to find the value for.
		\returns		The value assigned to the input item. If the item is not found, returns -1
		\see		https://github.com/AneurinSmith/win32-template/wiki/file#find
	*/
	int find(string item);

	/*
		\brief		The function assigns val to the row item if found. Also works as isSet() if the val param is not set.
		\param			item – The value to search for.
		\param			val – The value to assign to the item if found. If the value is NULL, dont set the value.
		\returns		True if the item exists, false if not. Can therefore be used to indicate if the row exists.
		\see		https://github.com/AneurinSmith/win32-template/wiki/file#set
	*/
	bool set(string item, const char* val);

	/*
		\brief		Saves the information stored. The path must be set using the 'load()' method, even if the file doesn't exist.
		\returns		Returns true if the file is successfully saved, otherwise returns false.
		\see		https://github.com/AneurinSmith/win32-template/wiki/file#save
	*/
	bool save();

};
