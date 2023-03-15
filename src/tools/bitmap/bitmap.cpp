
#include "bitmap.h"

Bitmap::Bitmap(HDC _hdc, HBITMAP _hbm) {
	hdc = _hdc;
	hbm = _hbm;
}

BOOL Bitmap::load(SIZE _size) {

	BITMAPINFOHEADER bi = getBI(_size);
	HDC outDC = CreateCompatibleDC(NULL);
	HBITMAP outHBM = CreateDIBSection(outDC, (BITMAPINFO*)&bi, DIB_RGB_COLORS, NULL, NULL, NULL);
	
	hbm = outHBM;

	return true;
}

BOOL Bitmap::load(HWND _hwnd) {

	RECT rc;
	GetClientRect(_hwnd, &rc);

	HDC wndDC = GetDC(_hwnd);
	hdc = CreateCompatibleDC(wndDC);
	hbm = CreateCompatibleBitmap(wndDC, rc.right, rc.bottom);

	SelectObject(hdc, hbm);
	BitBlt(hdc, 0, 0, rc.right, rc.bottom, wndDC, 0, 0, SRCCOPY);

	DeleteDC(wndDC);

	return true;
}

BOOL Bitmap::load(HDC _hdc, HBITMAP _hbm) {
	hdc = _hdc;
	hbm = _hbm;
	SelectObject(hdc, hbm);
	return true;
}


INT Bitmap::write(HDC _hdc) {

	return 0;
}

BOOL Bitmap::close() {
	DeleteObject(hbm);
	DeleteDC(hdc);
	return true;
}

BOOL Bitmap::getDDB(HBITMAP* out) {

	if (isDIB(hbm)) {

	}
	else {
		return true;
	}
}

BOOL Bitmap::getDIB(HBITMAP* out) {

	BITMAPINFOHEADER test2;
	GetObject(hbm, sizeof(test2), &test2);

	SIZE src_size = { test2.biWidth,test2.biHeight };
	BITMAPINFOHEADER bi = getBI(src_size);

	HDC outDC = CreateCompatibleDC(NULL);
	HBITMAP outHBM = CreateDIBSection(outDC, (BITMAPINFO*)&bi, DIB_RGB_COLORS, NULL, NULL, NULL);


	DWORD dwBmpSize = ((src_size.cx * bi.biBitCount + 31) / 32) * 4 * src_size.cy;
	HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
	char* lpbitmap = (char*)GlobalLock(hDIB);

	GetDIBits(hdc, hbm, 0, src_size.cy, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	SetDIBits(outDC, outHBM, 0, src_size.cy, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	BOOL test = isDIB(outHBM);

	*out = outHBM;


	GlobalUnlock(hDIB);
	GlobalFree(hDIB);
	DeleteDC(outDC);
	//DeleteObject(outHBM);

	return true;
}

BITMAPINFOHEADER Bitmap::getBI(SIZE size) {
	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(bi);
	bi.biWidth = size.cx;
	bi.biHeight = -size.cy;
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	return bi;
}

BOOL Bitmap::isDIB(HBITMAP _hbm) {
	DIBSECTION ds;
	GetObject(_hbm, sizeof(ds), &ds);
	return ds.dsBm.bmBits == 0 ? 0 : 1;
}