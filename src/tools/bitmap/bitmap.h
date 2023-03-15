
#include "include.h"

class Bitmap {
protected:
	HDC hdc;
	HBITMAP hbm;

public:
	Bitmap() = default;
	Bitmap(HDC, HBITMAP);		// Loads as DDB
	Bitmap(LPSTR);				// Loads as DIB

	BOOL load(HWND);			// Loads a DDB of the window
	BOOL load(HDC, HBITMAP);	// Loads as DDB
	BOOL load(LPSTR);			// Loads as DIB
	BOOL load(SIZE);
	INT  write(HDC);
	BOOL scale(SIZE scale);

	BOOL getDDB(HBITMAP*);		// Sends as DDB
	BOOL getDIB(HBITMAP*);		// Sends as DIB

	BOOL close();

private:
	BITMAPINFOHEADER getBI(SIZE);
	BOOL isDIB(HBITMAP);

};